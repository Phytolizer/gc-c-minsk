#include "SyntaxNode.h"

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

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

void syntax_node_free(struct SyntaxNode* node)
{
  switch (node->kind)
  {
    case SYNTAX_NODE_KIND_TOKEN:
      syntax_token_free((struct SyntaxToken*)node);
      break;
    case SYNTAX_NODE_KIND_EXPRESSION:
      expression_syntax_free((struct ExpressionSyntax*)node);
      break;
  }
}