#include "BoundExpressionStatement.h"

#include "BoundStatement.h"

struct BoundExpressionStatement *bound_expression_statement_new(struct BoundExpression *expression)
{
    struct BoundExpressionStatement *stmt = mc_malloc(sizeof(struct BoundExpressionStatement));
    bound_statement_init(&stmt->base, BOUND_STATEMENT_KIND_EXPRESSION);
    stmt->expression = expression;
    return stmt;
}

enum BoundNodeKind bound_expression_statement_get_kind(struct BoundExpressionStatement *stmt)
{
    (void)stmt;
    return BOUND_NODE_KIND_EXPRESSION_STATEMENT;
}
