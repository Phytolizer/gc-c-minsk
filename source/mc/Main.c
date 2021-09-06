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
  struct VariableStore* variables = variable_store_new();
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
    struct EvaluationResult* result
        = compilation_evaluate(compilation, variables);
    struct DiagnosticList* diagnostics = result->diagnostics;
    if (show_tree)
    {
      printf("\x1b[2;37m");
      syntax_node_pretty_print(stdout, (struct SyntaxNode*)tree->root);
      printf("\x1b[0m");
    }
    if (diagnostics->length > 0)
    {
      struct SourceText* text = tree->source_text;

      for (long i = 0; i < diagnostics->length; ++i)
      {
        int line_index = source_text_get_line_index(
            text,
            diagnostics->data[i]->span->start);
        int line_number = line_index + 1;
        int character = diagnostics->data[i]->span->start
            - text->lines->data[line_index]->start + 1;

        printf(
            "\n\x1b[31m(%d, %d): %s\x1b[0m\n",
            line_number,
            character,
            diagnostics->data[i]->message);
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
