#pragma once

#include <minsk/CodeAnalysis/Binding/BoundExpression.h>

#include "BoundUnaryOperator.h"

struct BoundUnaryExpression
{
  struct BoundExpression base;
  struct BoundUnaryOperator* op;
  struct BoundExpression* operand;
};

struct BoundUnaryExpression* bound_unary_expression_new(
    struct BoundUnaryOperator* op,
    struct BoundExpression* operand);
enum ObjectKind bound_unary_expression_get_type(
    struct BoundUnaryExpression* expr);
enum BoundNodeKind bound_unary_expression_get_kind(
    struct BoundUnaryExpression* expr);
