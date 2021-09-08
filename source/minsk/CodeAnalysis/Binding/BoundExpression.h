#pragma once

#include <common/Object.h>

#include "BoundNode.h"

#define BOUND_EXPRESSIONS_ \
    X(BOUND_BINARY_EXPRESSION) \
    X(BOUND_LITERAL_EXPRESSION) \
    X(BOUND_UNARY_EXPRESSION) \
    X(BOUND_VARIABLE_EXPRESSION) \
    X(BOUND_ASSIGNMENT_EXPRESSION)

enum BoundExpressionKind
{
#define X(x) BOUND_EXPRESSION_KIND_##x,
    BOUND_EXPRESSIONS_
#undef X
};

struct BoundExpression
{
    struct BoundNode base;
    enum BoundExpressionKind kind;
};

void bound_expression_init(struct BoundExpression* expr, enum BoundExpressionKind kind);
enum ObjectKind bound_expression_get_type(struct BoundExpression* expr);
enum BoundNodeKind bound_expression_get_kind(struct BoundExpression* expr);
