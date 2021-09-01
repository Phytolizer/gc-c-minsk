#include "minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h"

struct ParenthesizedExpressionSyntax* parenthesized_expression_syntax_new(
    struct SyntaxToken* open_parenthesis_token,
    struct ExpressionSyntax* expression,
    struct SyntaxToken* close_parenthesis_token)
{
  struct ParenthesizedExpressionSyntax* syntax
      = mc_malloc(sizeof(struct ParenthesizedExpressionSyntax));
  expression_syntax_init(
      (struct ExpressionSyntax*)syntax,
      EXPRESSION_SYNTAX_KIND_PARENTHESIZED_EXPRESSION_SYNTAX);
  syntax->open_parenthesis_token = open_parenthesis_token;
  syntax->expression = expression;
  syntax->close_parenthesis_token = close_parenthesis_token;
  return syntax;
}

enum SyntaxKind parenthesized_expression_syntax_get_kind(
    struct ParenthesizedExpressionSyntax* syntax)
{
  return SYNTAX_KIND_PARENTHESIZED_EXPRESSION;
}

struct SyntaxNodeList* parenthesized_expression_syntax_get_children(
    struct ParenthesizedExpressionSyntax* syntax)
{
  struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
  LIST_INIT(children);
  LIST_PUSH(children, (struct SyntaxNode*)syntax->open_parenthesis_token);
  LIST_PUSH(children, (struct SyntaxNode*)syntax->expression);
  LIST_PUSH(children, (struct SyntaxNode*)syntax->close_parenthesis_token);
  return children;
}
