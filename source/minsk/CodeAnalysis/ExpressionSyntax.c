#include "minsk/CodeAnalysis/ExpressionSyntax.h"

#include <minsk/CodeAnalysis/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/ParenthesizedExpressionSyntax.h>

void expression_syntax_init(
    struct ExpressionSyntax* syntax,
    enum ExpressionSyntaxKind kind)
{
  syntax->base.kind = SYNTAX_NODE_KIND_EXPRESSION;
  syntax->kind = kind;
}

enum SyntaxKind expression_syntax_get_kind(struct ExpressionSyntax* syntax)
{
  switch (syntax->kind)
  {
    case EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX:
      return binary_expression_syntax_get_kind(
          (struct BinaryExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_LITERAL_EXPRESSION_SYNTAX:
      return literal_expression_syntax_get_kind(
          (struct LiteralExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_PARENTHESIZED_EXPRESSION_SYNTAX:
      return parenthesized_expression_syntax_get_kind(
          (struct ParenthesizedExpressionSyntax*)syntax);
  }
}

struct SyntaxNodeList* expression_syntax_get_children(
    struct ExpressionSyntax* syntax)
{
  switch (syntax->kind)
  {
    case EXPRESSION_SYNTAX_KIND_LITERAL_EXPRESSION_SYNTAX:
      return literal_expression_syntax_get_children(
          (struct LiteralExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX:
      return binary_expression_syntax_get_children(
          (struct BinaryExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_PARENTHESIZED_EXPRESSION_SYNTAX:
      return parenthesized_expression_syntax_get_children(
          (struct ParenthesizedExpressionSyntax*)syntax);
  }
}
