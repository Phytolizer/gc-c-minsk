#pragma once

#include <common/Object.h>
#include <minsk/CodeAnalysis/Binding/BoundExpression.h>

struct BoundLiteralExpression
{
  struct BoundExpression base;
  struct Object* value;
};

struct BoundLiteralExpression* bound_literal_expression_new(
    struct Object* value);
enum ObjectKind bound_literal_expression_get_type(
    struct BoundLiteralExpression* expr);
enum BoundNodeKind bound_literal_expression_get_kind(
    struct BoundLiteralExpression* expr);
