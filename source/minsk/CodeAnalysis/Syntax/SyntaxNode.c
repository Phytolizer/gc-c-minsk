#include "minsk/CodeAnalysis/Syntax/SyntaxNode.h"

#include <minsk/CodeAnalysis/Syntax/ExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>

const char* const SYNTAX_NODE_KINDS[] = {
#define X(x) #x,
    SYNTAX_NODE_KINDS_
#undef X
};

static void pretty_print(
    FILE* stream,
    struct SyntaxNode* node,
    sds indent,
    bool is_last);

enum SyntaxKind syntax_node_get_kind(struct SyntaxNode* node)
{
  switch (node->kind)
  {
    case SYNTAX_NODE_KIND_TOKEN:
      return syntax_token_get_kind((struct SyntaxToken*)node);
    case SYNTAX_NODE_KIND_EXPRESSION:
      return expression_syntax_get_kind((struct ExpressionSyntax*)node);
  }
}

struct SyntaxNodeList* syntax_node_get_children(struct SyntaxNode* node)
{
  switch (node->kind)
  {
    case SYNTAX_NODE_KIND_TOKEN:
      return syntax_token_get_children((struct SyntaxToken*)node);
    case SYNTAX_NODE_KIND_EXPRESSION:
      return expression_syntax_get_children((struct ExpressionSyntax*)node);
  }
}

struct TextSpan* syntax_node_get_span(struct SyntaxNode* node)
{
  switch (node->kind)
  {
    case SYNTAX_NODE_KIND_TOKEN:
      return syntax_token_get_span((struct SyntaxToken*)node);
    case SYNTAX_NODE_KIND_EXPRESSION:
      break;
  }
  struct SyntaxNodeList* children = syntax_node_get_children(node);
  struct TextSpan* first = syntax_node_get_span(children->data[0]);
  struct TextSpan* last
      = syntax_node_get_span(children->data[children->length - 1]);
  return text_span_from_bounds(first->start, text_span_end(last));
}

void syntax_node_pretty_print(FILE* stream, struct SyntaxNode* node)
{
  pretty_print(stream, node, sdsempty(), true);
}

static void pretty_print(
    FILE* stream,
    struct SyntaxNode* node,
    sds indent,
    bool is_last)
{
  bool is_to_stdout = stream == stdout;
  sds marker = is_last ? sdsnew("└───") : sdsnew("├───");
  if (is_to_stdout)
  {
    fprintf(stream, "\x1b[2;37m");
  }
  fprintf(stream, "%s", indent);
  if (is_to_stdout)
  {
    fprintf(stream, "%s", marker);
    fprintf(stream, "\x1b[0m");
  }
  if (is_to_stdout)
  {
    fprintf(
        stream,
        "%s",
        node->kind == SYNTAX_NODE_KIND_TOKEN ? "\x1b[34m" : "\x1b[36m");
  }
  fprintf(stream, "%s", SYNTAX_KINDS[syntax_node_get_kind(node)]);
  if (is_to_stdout)
  {
    fprintf(stream, "\x1b[0m");
  }

  if (node->kind == SYNTAX_NODE_KIND_TOKEN
      && ((struct SyntaxToken*)node)->value->kind != OBJECT_KIND_NULL)
  {
    fprintf(
        stream,
        " %s",
        object_to_string(((struct SyntaxToken*)node)->value));
  }
  fprintf(stream, "\n");
  indent = sdsdup(indent);
  indent = sdscat(indent, is_last ? "    " : "│   ");
  struct SyntaxNodeList* children = syntax_node_get_children(node);
  for (long i = 0; i < children->length; i++)
  {
    pretty_print(stream, children->data[i], indent, i == children->length - 1);
  }
}
