#include "BoundVariableExpression.h"

#include <IncludeMe.h>

struct BoundVariableExpression* bound_variable_expression_new(
    sds name,
    enum ObjectKind type)
{
  struct BoundVariableExpression* expression
      = mc_malloc(sizeof(struct BoundVariableExpression));
  bound_expression_init(
      (struct BoundExpression*)expression,
      BOUND_EXPRESSION_KIND_BOUND_VARIABLE_EXPRESSION);
  expression->name = name;
  expression->type = type;
  return expression;
}

enum BoundNodeKind bound_variable_expression_get_kind(
    struct BoundVariableExpression* expression)
{
  (void)expression;
  return BOUND_NODE_KIND_VARIABLE_EXPRESSION;
}

enum ObjectKind bound_variable_expression_get_type(
    struct BoundVariableExpression* expression)
{
  return expression->type;
}
