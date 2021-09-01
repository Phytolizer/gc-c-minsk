#include "BoundUnaryExpression.h"

#include <IncludeMe.h>

struct BoundUnaryExpression* bound_unary_expression_new(
    enum BoundUnaryOperatorKind operator_kind,
    struct BoundExpression* operand)
{
  struct BoundUnaryExpression* expr
      = mc_malloc(sizeof(struct BoundUnaryExpression));
  bound_expression_init(
      (struct BoundExpression*)expr,
      BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION);
  expr->operator_kind = operator_kind;
  expr->operand = operand;
  return expr;
}

enum ObjectKind bound_unary_expression_get_type(
    struct BoundUnaryExpression* expr)
{
  return bound_expression_get_type(expr->operand);
}

enum BoundNodeKind bound_unary_expression_get_kind(
    struct BoundUnaryExpression* expr)
{
  return BOUND_NODE_KIND_UNARY_EXPRESSION;
}