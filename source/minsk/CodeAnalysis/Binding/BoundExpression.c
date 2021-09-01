#include "minsk/CodeAnalysis/Binding/BoundExpression.h"

#include "BoundBinaryExpression.h"
#include "BoundLiteralExpression.h"
#include "BoundUnaryExpression.h"

void bound_expression_init(
    struct BoundExpression* expr,
    enum BoundExpressionKind kind)
{
  expr->base.type = BOUND_NODE_TYPE_EXPRESSION;
  expr->kind = kind;
}

enum ObjectKind bound_expression_get_type(struct BoundExpression* expr)
{
  switch (expr->kind)
  {
    case BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION:
      return bound_unary_expression_get_type(
          (struct BoundUnaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_LITERAL_EXPRESSION:
      return bound_literal_expression_get_type(
          (struct BoundLiteralExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_BINARY_EXPRESSION:
      return bound_binary_expression_get_type(
          (struct BoundBinaryExpression*)expr);
  }
}

enum BoundNodeKind bound_expression_get_kind(struct BoundExpression* expr)
{
  switch (expr->kind)
  {
    case BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION:
      return bound_unary_expression_get_kind(
          (struct BoundUnaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_LITERAL_EXPRESSION:
      return bound_literal_expression_get_kind(
          (struct BoundLiteralExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_BINARY_EXPRESSION:
      return bound_binary_expression_get_kind(
          (struct BoundBinaryExpression*)expr);
  }
}
