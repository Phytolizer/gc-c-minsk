#include "minsk/CodeAnalysis/NumberExpressionSyntax.h"

#include "IncludeMe.h"

struct NumberExpressionSyntax* number_expression_syntax_new(
    struct SyntaxToken* number_token)
{
  struct NumberExpressionSyntax* syntax
      = mc_malloc(sizeof(struct NumberExpressionSyntax));
  expression_syntax_init(
      (struct ExpressionSyntax*)syntax,
      EXPRESSION_SYNTAX_KIND_NUMBER_EXPRESSION_SYNTAX);
  syntax->number_token = number_token;
  return syntax;
}

void number_expression_syntax_free(struct NumberExpressionSyntax* syntax)
{
  syntax_token_free(syntax->number_token);
  mc_free(syntax);
}

enum SyntaxKind number_expression_syntax_get_kind(
    struct NumberExpressionSyntax* syntax)
{
  (void)syntax;
  return SYNTAX_KIND_NUMBER_EXPRESSION;
}

struct SyntaxNodeList* number_expression_syntax_get_children(
    struct NumberExpressionSyntax* syntax)
{
  struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
  LIST_INIT(children);
  LIST_PUSH(children, (struct SyntaxNode*)syntax->number_token);
  return children;
}
