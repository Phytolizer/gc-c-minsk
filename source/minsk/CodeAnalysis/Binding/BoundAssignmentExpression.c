#include "BoundAssignmentExpression.h"

#include <IncludeMe.h>

struct BoundAssignmentExpression* bound_assignment_expression_new(
    struct VariableSymbol* name, struct BoundExpression* expression)
{
    struct BoundAssignmentExpression* expr = mc_malloc(sizeof(struct BoundAssignmentExpression));
    bound_expression_init((struct BoundExpression*)expr, BOUND_EXPRESSION_KIND_BOUND_ASSIGNMENT_EXPRESSION);
    expr->variable = name;
    expr->expression = expression;
    return expr;
}

enum BoundNodeKind bound_assignment_expression_get_kind(struct BoundAssignmentExpression* expression)
{
    (void)expression;
    return BOUND_NODE_KIND_ASSIGNMENT_EXPRESSION;
}

enum ObjectKind bound_assignment_expression_get_type(struct BoundAssignmentExpression* expression)
{
    return bound_expression_get_type(expression->expression);
}

struct BoundNodeList* bound_assignment_expression_get_children(struct BoundAssignmentExpression* expression)
{
    struct BoundNodeList* children = mc_malloc(sizeof(struct BoundNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct BoundNode*)expression->expression);
    return children;
}
