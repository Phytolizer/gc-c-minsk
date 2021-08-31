#include "SyntaxNode.h"

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

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
