#include "BinaryExpressionSyntax.h"

#include <gc.h>

struct BinaryExpressionSyntax* binary_expression_syntax_new(
    struct ExpressionSyntax* left,
    struct SyntaxToken* operator_token,
    struct ExpressionSyntax* right)
{
  struct BinaryExpressionSyntax* syntax
      = GC_MALLOC(sizeof(struct BinaryExpressionSyntax));
  expression_syntax_init(
      (struct ExpressionSyntax*)syntax,
      EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX);
  syntax->left = left;
  syntax->operator_token = operator_token;
  syntax->right = right;
  return syntax;
}

enum SyntaxKind binary_expression_syntax_get_kind(
    struct BinaryExpressionSyntax* syntax)
{
  return SYNTAX_KIND_BINARY_EXPRESSION;
}
