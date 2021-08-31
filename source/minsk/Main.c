#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "IncludeMe.h"

#include "Parser.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "sds.h"

static void pretty_print(struct SyntaxNode* node, sds indent);

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
    struct ExpressionSyntax* root = parser_parse(parser);
    pretty_print((struct SyntaxNode*)root, sdsempty());
  }
  return 0;
}

static void pretty_print(struct SyntaxNode* node, sds indent)
{
  printf("%s%s", indent, SYNTAX_KINDS[syntax_node_get_kind(node)]);

  if (node->kind == SYNTAX_NODE_KIND_TOKEN
      && ((struct SyntaxToken*)node)->value->kind != OBJECT_KIND_NULL)
  {
    printf(" %s", object_to_string(((struct SyntaxToken*)node)->value));
  }
  printf("\n");
  indent = sdsdup(indent);
  indent = sdscat(indent, "    ");
  struct SyntaxNodeList* children = syntax_node_get_children(node);
  for (long i = 0; i < children->length; i++)
  {
    pretty_print(children->data[i], indent);
  }
  sdsfree(indent);
}
