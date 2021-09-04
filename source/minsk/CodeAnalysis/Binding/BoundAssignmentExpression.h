#pragma once

#include <sds.h>

#include "BoundExpression.h"

struct BoundAssignmentExpression
{
  struct BoundExpression base;
  sds name;
  struct BoundExpression* expression;
};

struct BoundAssignmentExpression* bound_assignment_expression_new(
    sds name,
    struct BoundExpression* expression);
enum BoundNodeKind bound_assignment_expression_get_kind(
    struct BoundAssignmentExpression* expression);
enum ObjectKind bound_assignment_expression_get_type(
    struct BoundAssignmentExpression* expression);
