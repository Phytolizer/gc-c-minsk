#include "NumberExpressionSyntax.h"

#include <gc.h>

struct NumberExpressionSyntax* number_expression_syntax_new(
    struct SyntaxToken* number_token)
{
  struct NumberExpressionSyntax* syntax
      = GC_MALLOC(sizeof(struct NumberExpressionSyntax));
  expression_syntax_init(
      (struct ExpressionSyntax*)syntax,
      EXPRESSION_SYNTAX_KIND_NUMBER_EXPRESSION_SYNTAX);
  syntax->number_token = number_token;
  return syntax;
}

enum SyntaxKind number_expression_syntax_get_kind(
    struct NumberExpressionSyntax* syntax)
{
  (void)syntax;
  return SYNTAX_KIND_NUMBER_EXPRESSION;
}
