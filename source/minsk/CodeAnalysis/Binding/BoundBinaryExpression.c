#include "BoundBinaryExpression.h"

#include <IncludeMe.h>

struct BoundBinaryExpression* bound_binary_expression_new(
    struct BoundExpression* left, struct BoundBinaryOperator* op, struct BoundExpression* right)
{
    struct BoundBinaryExpression* expr = mc_malloc(sizeof(struct BoundBinaryExpression));
    bound_expression_init((struct BoundExpression*)expr, BOUND_EXPRESSION_KIND_BOUND_BINARY_EXPRESSION);
    expr->left = left;
    expr->op = op;
    expr->right = right;
    return expr;
}

enum ObjectKind bound_binary_expression_get_type(struct BoundBinaryExpression* expr)
{
    return expr->op->result_type;
}

enum BoundNodeKind bound_binary_expression_get_kind(struct BoundBinaryExpression* expr)
{
    (void)expr;
    return BOUND_NODE_KIND_BINARY_EXPRESSION;
}
