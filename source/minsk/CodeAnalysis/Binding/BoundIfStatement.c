#include "BoundIfStatement.h"

struct BoundIfStatement* bound_if_statement_new(struct BoundExpression* condition,
                                                struct BoundStatement* then_statement,
                                                struct BoundStatement* else_statement)
{
    struct BoundIfStatement* stmt = mc_malloc(sizeof(struct BoundIfStatement));
    bound_statement_init(&stmt->base, BOUND_STATEMENT_KIND_IF);
    stmt->condition = condition;
    stmt->then_statement = then_statement;
    stmt->else_statement = else_statement;
    return stmt;
}

enum BoundNodeKind bound_if_statement_get_kind(struct BoundIfStatement* stmt)
{
    (void)stmt;
    return BOUND_NODE_KIND_IF_STATEMENT;
}
