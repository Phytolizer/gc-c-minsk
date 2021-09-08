#pragma once

#include <common/Object.h>

#include "BoundBinaryOperator.h"
#include "BoundExpression.h"

struct BoundBinaryExpression
{
    struct BoundExpression base;
    struct BoundExpression* left;
    struct BoundBinaryOperator* op;
    struct BoundExpression* right;
};

struct BoundBinaryExpression* bound_binary_expression_new(struct BoundExpression* left, struct BoundBinaryOperator* op,
                                                          struct BoundExpression* right);
enum ObjectKind bound_binary_expression_get_type(struct BoundBinaryExpression* expr);
enum BoundNodeKind bound_binary_expression_get_kind(struct BoundBinaryExpression* expr);
