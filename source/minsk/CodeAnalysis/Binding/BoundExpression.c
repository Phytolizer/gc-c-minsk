#include "BoundExpression.h"

#include "BoundAssignmentExpression.h"
#include "BoundBinaryExpression.h"
#include "BoundLiteralExpression.h"
#include "BoundUnaryExpression.h"
#include "BoundVariableExpression.h"

void bound_expression_init(struct BoundExpression* expr, enum BoundExpressionKind kind)
{
    expr->base.type = BOUND_NODE_TYPE_EXPRESSION;
    expr->kind = kind;
}

enum ObjectKind bound_expression_get_type(struct BoundExpression* expr)
{
    switch (expr->kind)
    {
    case BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION:
        return bound_unary_expression_get_type((struct BoundUnaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_LITERAL_EXPRESSION:
        return bound_literal_expression_get_type((struct BoundLiteralExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_BINARY_EXPRESSION:
        return bound_binary_expression_get_type((struct BoundBinaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_VARIABLE_EXPRESSION:
        return bound_variable_expression_get_type((struct BoundVariableExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_ASSIGNMENT_EXPRESSION:
        return bound_assignment_expression_get_type((struct BoundAssignmentExpression*)expr);
    }
}

enum BoundNodeKind bound_expression_get_kind(struct BoundExpression* expr)
{
    switch (expr->kind)
    {
    case BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION:
        return bound_unary_expression_get_kind((struct BoundUnaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_LITERAL_EXPRESSION:
        return bound_literal_expression_get_kind((struct BoundLiteralExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_BINARY_EXPRESSION:
        return bound_binary_expression_get_kind((struct BoundBinaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_VARIABLE_EXPRESSION:
        return bound_variable_expression_get_kind((struct BoundVariableExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_ASSIGNMENT_EXPRESSION:
        return bound_assignment_expression_get_kind((struct BoundAssignmentExpression*)expr);
    }
}

struct BoundNodeList* bound_expression_get_children(struct BoundExpression* expr)
{
    switch (expr->kind)
    {
    case BOUND_EXPRESSION_KIND_BOUND_BINARY_EXPRESSION:
        return bound_binary_expression_get_children((struct BoundBinaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_LITERAL_EXPRESSION:
        return bound_literal_expression_get_children((struct BoundLiteralExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION:
        return bound_unary_expression_get_children((struct BoundUnaryExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_VARIABLE_EXPRESSION:
        return bound_variable_expression_get_children((struct BoundVariableExpression*)expr);
    case BOUND_EXPRESSION_KIND_BOUND_ASSIGNMENT_EXPRESSION:
        return bound_assignment_expression_get_children((struct BoundAssignmentExpression*)expr);
    }
}
