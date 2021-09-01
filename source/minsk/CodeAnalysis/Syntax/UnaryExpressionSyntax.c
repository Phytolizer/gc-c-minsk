#include "minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h"

struct UnaryExpressionSyntax* unary_expression_syntax_new(
    struct SyntaxToken* operator_token,
    struct ExpressionSyntax* operand)
{
  struct UnaryExpressionSyntax* syntax
      = mc_malloc(sizeof(struct UnaryExpressionSyntax));
  expression_syntax_init(
      (struct ExpressionSyntax*)syntax,
      EXPRESSION_SYNTAX_KIND_UNARY_EXPRESSION_SYNTAX);
  syntax->operator_token = operator_token;
  syntax->operand = operand;
  return syntax;
}

enum SyntaxKind unary_expression_syntax_get_kind(
    struct UnaryExpressionSyntax* syntax)
{
  return SYNTAX_KIND_UNARY_EXPRESSION;
}

struct SyntaxNodeList* unary_expression_syntax_get_children(
    struct UnaryExpressionSyntax* syntax)
{
  struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
  LIST_INIT(children);
  LIST_PUSH(children, (struct SyntaxNode*)syntax->operator_token);
  LIST_PUSH(children, (struct SyntaxNode*)syntax->operand);
  return children;
}
