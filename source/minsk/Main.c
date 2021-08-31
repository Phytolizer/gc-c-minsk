#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "IncludeMe.h"
#include "Parser.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "sds.h"

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
  while (true)
  {
    sds line = input_line("> ");
    if (!line)
    {
      printf("\n");
      break;
    }

    struct Parser* parser = parser_new(line);
    struct SyntaxTree* tree = parser_parse(parser);
    if (parser->diagnostics->length > 0)
    {
      for (long i = 0; i < parser->diagnostics->length; ++i)
      {
        printf("%s\n", parser->diagnostics->data[i]);
      }
    }
    pretty_print((struct SyntaxNode*)tree->root, sdsempty(), true);
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
