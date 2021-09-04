#pragma once

#include <sds.h>

#include "BoundExpression.h"

struct BoundVariableExpression
{
  struct BoundExpression base;
  sds name;
  enum ObjectKind type;
};

struct BoundVariableExpression* bound_variable_expression_new(
    sds name,
    enum ObjectKind type);
enum BoundNodeKind bound_variable_expression_get_kind(
    struct BoundVariableExpression* expression);
enum ObjectKind bound_variable_expression_get_type(
    struct BoundVariableExpression* expression);
