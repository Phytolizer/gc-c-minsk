#include "minsk-private/CodeAnalysis/Binding/BoundExpressionStatement.h"

struct BoundExpressionStatement* bound_expression_statement_new(struct BoundExpression* expression)
{
    struct BoundExpressionStatement* stmt = mc_malloc(sizeof(struct BoundExpressionStatement));
    bound_statement_init(&stmt->base, BOUND_STATEMENT_KIND_EXPRESSION);
    stmt->expression = expression;
    return stmt;
}

enum BoundNodeKind bound_expression_statement_get_kind(struct BoundExpressionStatement* stmt)
{
    (void)stmt;
    return BOUND_NODE_KIND_EXPRESSION_STATEMENT;
}

struct BoundNodeList* bound_expression_statement_get_children(struct BoundExpressionStatement* stmt)
{
    struct BoundNodeList* children = mc_malloc(sizeof(struct BoundNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct BoundNode*)stmt->expression);
    return children;
}
