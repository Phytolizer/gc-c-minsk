#include "BoundUnaryExpression.h"

#include <IncludeMe.h>

struct BoundUnaryExpression* bound_unary_expression_new(
    struct BoundUnaryOperator* op,
    struct BoundExpression* operand)
{
  struct BoundUnaryExpression* expr
      = mc_malloc(sizeof(struct BoundUnaryExpression));
  bound_expression_init(
      (struct BoundExpression*)expr,
      BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION);
  expr->op = op;
  expr->operand = operand;
  return expr;
}

enum ObjectKind bound_unary_expression_get_type(
    struct BoundUnaryExpression* expr)
{
  return expr->op->result_type;
}

enum BoundNodeKind bound_unary_expression_get_kind(
    struct BoundUnaryExpression* expr)
{
  (void)expr;
  return BOUND_NODE_KIND_UNARY_EXPRESSION;
}
