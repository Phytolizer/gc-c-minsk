#include "ExpressionSyntax.h"

#include "BinaryExpressionSyntax.h"
#include "NumberExpressionSyntax.h"

void expression_syntax_init(
    struct ExpressionSyntax* syntax,
    enum ExpressionSyntaxKind kind)
{
  syntax->base.kind = SYNTAX_NODE_KIND_EXPRESSION;
  syntax->kind = kind;
}

void expression_syntax_free(struct ExpressionSyntax* syntax)
{
  switch (syntax->kind) {
    case EXPRESSION_SYNTAX_KIND_NUMBER_EXPRESSION_SYNTAX:
      number_expression_syntax_free((struct NumberExpressionSyntax*)syntax);
      break;
    case EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX:
      binary_expression_syntax_free((struct BinaryExpressionSyntax*)syntax);
      break;
  }
}

enum SyntaxKind expression_syntax_get_kind(struct ExpressionSyntax* syntax)
{
  switch (syntax->kind)
  {
    case EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX:
      return binary_expression_syntax_get_kind(
          (struct BinaryExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_NUMBER_EXPRESSION_SYNTAX:
      return number_expression_syntax_get_kind(
          (struct NumberExpressionSyntax*)syntax);
  }
}

struct SyntaxNodeList* expression_syntax_get_children(
    struct ExpressionSyntax* syntax)
{
  switch (syntax->kind)
  {
    case EXPRESSION_SYNTAX_KIND_NUMBER_EXPRESSION_SYNTAX:
      return number_expression_syntax_get_children(
          (struct NumberExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX:
      return binary_expression_syntax_get_children(
          (struct BinaryExpressionSyntax*)syntax);
  }
}
