#pragma once

#include <common/Object.h>
#include <minsk/CodeAnalysis/Binding/BoundExpression.h>

#include "BoundBinaryOperatorKind.h"

struct BoundBinaryExpression
{
  struct BoundExpression base;
  struct BoundExpression* left;
  enum BoundBinaryOperatorKind operator_kind;
  struct BoundExpression* right;
};

struct BoundBinaryExpression* bound_binary_expression_new(
    struct BoundExpression* left,
    enum BoundBinaryOperatorKind operator_kind,
    struct BoundExpression* right);
enum ObjectKind bound_binary_expression_get_type(
    struct BoundBinaryExpression* expr);
enum BoundNodeKind bound_binary_expression_get_kind(
    struct BoundBinaryExpression* expr);
