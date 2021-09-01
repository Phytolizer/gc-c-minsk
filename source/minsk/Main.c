#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <IncludeMe.h>
#include <minsk/CodeAnalysis/Binding/Binder.h>
#include <minsk/CodeAnalysis/Evaluator.h>
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
    struct StringList* diagnostics = tree->diagnostics;
    struct Binder* b = binder_new();
    struct BoundExpression* bound_expression = binder_bind(b, tree->root);
    for (long i = 0; i < b->diagnostics->length; ++i)
    {
      LIST_PUSH(diagnostics, b->diagnostics->data[i]);
    }
    if (show_tree)
    {
      printf("\x1b[2;37m");
      pretty_print((struct SyntaxNode*)tree->root, sdsempty(), true);
      printf("\x1b[0m");
    }
    if (diagnostics->length > 0)
    {
      printf("\x1b[31m");
      for (long i = 0; i < diagnostics->length; ++i)
      {
        printf("%s\n", diagnostics->data[i]);
      }
      printf("\x1b[0m");
    }
    else
    {
      struct Evaluator* e = evaluator_new(bound_expression);
      int result = evaluator_evaluate(e);
      printf("%d\n", result);
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
