#include "minsk/CodeAnalysis/Syntax/ExpressionSyntax.h"

#include <minsk/CodeAnalysis/Syntax/AssignmentExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/NameExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>

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
    case EXPRESSION_SYNTAX_KIND_UNARY_EXPRESSION_SYNTAX:
      return unary_expression_syntax_get_kind(
          (struct UnaryExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_NAME_EXPRESSION_SYNTAX:
      return name_expression_syntax_get_kind(
          (struct NameExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_ASSIGNMENT_EXPRESSION_SYNTAX:
      return assignment_expression_syntax_get_kind(
          (struct AssignmentExpressionSyntax*)syntax);
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
    case EXPRESSION_SYNTAX_KIND_UNARY_EXPRESSION_SYNTAX:
      return unary_expression_syntax_get_children(
          (struct UnaryExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_NAME_EXPRESSION_SYNTAX:
      return name_expression_syntax_get_children(
          (struct NameExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_ASSIGNMENT_EXPRESSION_SYNTAX:
      return assignment_expression_syntax_get_children(
          (struct AssignmentExpressionSyntax*)syntax);
  }
}
