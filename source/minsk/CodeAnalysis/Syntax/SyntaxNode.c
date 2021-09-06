#include "minsk/CodeAnalysis/Syntax/SyntaxNode.h"

#include <minsk/CodeAnalysis/Syntax/ExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>

const char* const SYNTAX_NODE_KINDS[] = {
#define X(x) #x,
    SYNTAX_NODE_KINDS_
#undef X
};

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
