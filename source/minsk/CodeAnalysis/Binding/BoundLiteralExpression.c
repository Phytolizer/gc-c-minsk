#include "BoundLiteralExpression.h"

#include <IncludeMe.h>

struct BoundLiteralExpression* bound_literal_expression_new(struct Object* value)
{
    struct BoundLiteralExpression* expr = mc_malloc(sizeof(struct BoundLiteralExpression));
    bound_expression_init((struct BoundExpression*)expr, BOUND_EXPRESSION_KIND_BOUND_LITERAL_EXPRESSION);
    expr->value = value;
    return expr;
}

enum ObjectKind bound_literal_expression_get_type(struct BoundLiteralExpression* expr)
{
    return expr->value->kind;
}

enum BoundNodeKind bound_literal_expression_get_kind(struct BoundLiteralExpression* expr)
{
    (void)expr;
    return BOUND_NODE_KIND_LITERAL_EXPRESSION;
}
