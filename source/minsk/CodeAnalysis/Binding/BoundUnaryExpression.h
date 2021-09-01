#pragma once

#include <minsk/CodeAnalysis/Binding/BoundExpression.h>

#include "BoundUnaryOperatorKind.h"

struct BoundUnaryExpression
{
  struct BoundExpression base;
  enum BoundUnaryOperatorKind operator_kind;
  struct BoundExpression* operand;
};

struct BoundUnaryExpression* bound_unary_expression_new(
    enum BoundUnaryOperatorKind operator_kind,
    struct BoundExpression* operand);
enum ObjectKind bound_unary_expression_get_type(
    struct BoundUnaryExpression* expr);
enum BoundNodeKind bound_unary_expression_get_kind(
    struct BoundUnaryExpression* expr);
