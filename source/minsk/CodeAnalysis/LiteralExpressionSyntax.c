#include "minsk/CodeAnalysis/LiteralExpressionSyntax.h"

#include "IncludeMe.h"

struct LiteralExpressionSyntax* literal_expression_syntax_new(
    struct SyntaxToken* number_token)
{
  struct LiteralExpressionSyntax* syntax
      = mc_malloc(sizeof(struct LiteralExpressionSyntax));
  expression_syntax_init(
      (struct ExpressionSyntax*)syntax,
      EXPRESSION_SYNTAX_KIND_LITERAL_EXPRESSION_SYNTAX);
  syntax->literal_token = number_token;
  return syntax;
}

void number_expression_syntax_free(struct LiteralExpressionSyntax* syntax)
{
  syntax_token_free(syntax->literal_token);
  mc_free(syntax);
}

enum SyntaxKind literal_expression_syntax_get_kind(
    struct LiteralExpressionSyntax* syntax)
{
  (void)syntax;
  return SYNTAX_KIND_NUMBER_EXPRESSION;
}

struct SyntaxNodeList* literal_expression_syntax_get_children(
    struct LiteralExpressionSyntax* syntax)
{
  struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
  LIST_INIT(children);
  LIST_PUSH(children, (struct SyntaxNode*)syntax->literal_token);
  return children;
}
