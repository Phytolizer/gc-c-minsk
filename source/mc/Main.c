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
  sds text = sdsempty();
  while (true)
  {
    printf("\x1b[32m");
    if (sdslen(text) == 0)
    {
      printf("» ");
    }
    else
    {
      printf("· ");
    }
    printf("\x1b[0m");
    sds input = input_line("");
    bool is_blank = !input || sdslen(input) == 0;

    if (sdslen(text) == 0)
    {
      if (is_blank)
      {
        printf("\n");
        break;
      }
      if (strcmp(input, "#showTree") == 0)
      {
        show_tree = !show_tree;
        printf(
            "%s\n",
            show_tree ? "Showing parse trees." : "Not showing parse trees.");
        continue;
      }
      if (strcmp(input, "#cls") == 0)
      {
        printf("\x1b[2J");    // clear screen
        printf("\x1b[0;0H");    // move cursor
        continue;
      }
    }

    if (input)
    {
      text = sdscatfmt(text, "%S\n", input);
    }

    struct SyntaxTree* tree = syntax_tree_parse(text);
    if (!is_blank && tree->diagnostics->diagnostics->length > 0)
    {
      continue;
    }
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
        struct Diagnostic* diagnostic = diagnostics->data[i];
        int line_index
            = source_text_get_line_index(text, diagnostic->span->start);
        int line_number = line_index + 1;
        struct TextLine* line = text->lines->data[line_index];
        int character = diagnostic->span->start - line->start + 1;

        printf(
            "\n\x1b[31m(%d, %d): %s\x1b[0m\n",
            line_number,
            character,
            diagnostic->message);

        struct TextSpan* prefix_span
            = text_span_from_bounds(line->start, diagnostic->span->start);
        struct TextSpan* suffix_span = text_span_from_bounds(
            text_span_end(diagnostic->span),
            text_line_get_end(line));

        sds prefix = source_text_to_string_spanned(text, prefix_span);
        sds error = source_text_to_string_spanned(text, diagnostic->span);
        sds suffix = source_text_to_string_spanned(text, suffix_span);
        printf("    %s\x1b[31m%s\x1b[0m%s\n", prefix, error, suffix);
      }
      printf("\n");
    }
    else
    {
      printf("\x1b[35m%s\x1b[0m\n", object_to_string(result->value));
    }
    text = sdsempty();
  }
  return 0;
}
