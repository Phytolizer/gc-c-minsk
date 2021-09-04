#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <IncludeMe.h>
#include <minsk/CodeAnalysis/Compilation.h>
#include <minsk/CodeAnalysis/EvaluationResult.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxNode.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <sds.h>

static void pretty_print(struct SyntaxNode* node, sds indent, bool is_last);

sds input_line(const char* prompt)
{
  printf("%s", prompt);
  sds line = sdsempty();
  while (true)
  {
    char c = fgetc(stdin);
    if (c == EOF)
    {
      return NULL;
    }
    if (c == '\n' || c == '\0')
    {
      break;
    }
    line = sdscatlen(line, &c, 1);
  }
  return line;
}

int main(void)
{
  bool show_tree = false;
  while (true)
  {
    sds line = input_line("> ");
    if (!line)
    {
      printf("\n");
      break;
    }
    if (strcmp(line, "#showTree") == 0)
    {
      show_tree = !show_tree;
      printf(
          "%s\n",
          show_tree ? "Showing parse trees." : "Not showing parse trees.");
      continue;
    }
    if (strcmp(line, "#cls") == 0)
    {
      printf("\x1b[2J");    // clear screen
      printf("\x1b[0;0H");    // move cursor
      continue;
    }

    struct SyntaxTree* tree = syntax_tree_parse(line);
    struct Compilation* compilation = compilation_new(tree);
    struct EvaluationResult* result = compilation_evaluate(compilation);
    struct DiagnosticList* diagnostics = result->diagnostics;
    if (show_tree)
    {
      printf("\x1b[2;37m");
      pretty_print((struct SyntaxNode*)tree->root, sdsempty(), true);
      printf("\x1b[0m");
    }
    if (diagnostics->length > 0)
    {
      for (long i = 0; i < diagnostics->length; ++i)
      {
        printf("\n\x1b[31m%s\x1b[0m\n", diagnostics->data[i]->message);
        sds prefix
            = sdscatlen(sdsempty(), line, diagnostics->data[i]->span->start);
        sds error = sdscatlen(
            sdsempty(),
            &line[diagnostics->data[i]->span->start],
            diagnostics->data[i]->span->length);
        sds suffix = sdscat(
            sdsempty(),
            &line[text_span_end(diagnostics->data[i]->span)]);

        printf("    %s\x1b[31m%s\x1b[0m%s\n", prefix, error, suffix);
      }
      printf("\n");
    }
    else
    {
      printf("%s\n", object_to_string(result->value));
    }
  }
  return 0;
}

static void pretty_print(struct SyntaxNode* node, sds indent, bool is_last)
{
  sds marker = is_last ? sdsnew("└───") : sdsnew("├───");
  printf("%s%s%s", indent, marker, SYNTAX_KINDS[syntax_node_get_kind(node)]);

  if (node->kind == SYNTAX_NODE_KIND_TOKEN
      && ((struct SyntaxToken*)node)->value->kind != OBJECT_KIND_NULL)
  {
    printf(" %s", object_to_string(((struct SyntaxToken*)node)->value));
  }
  printf("\n");
  indent = sdsdup(indent);
  indent = sdscat(indent, is_last ? "    " : "│   ");
  struct SyntaxNodeList* children = syntax_node_get_children(node);
  for (long i = 0; i < children->length; i++)
  {
    pretty_print(children->data[i], indent, i == children->length - 1);
  }
  sdsfree(indent);
}
