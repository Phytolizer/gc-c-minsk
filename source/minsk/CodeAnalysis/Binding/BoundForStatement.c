#include "BoundForStatement.h"

struct BoundForStatement* bound_for_statement_new(
    struct VariableSymbol* variable,
    struct BoundExpression* lower_bound,
    struct BoundExpression* upper_bound,
    struct BoundStatement* body)
{
    struct BoundForStatement* stmt = mc_malloc(sizeof(struct BoundForStatement));
    bound_statement_init(&stmt->base, BOUND_STATEMENT_KIND_FOR);
    stmt->variable = variable;
    stmt->lower_bound = lower_bound;
    stmt->upper_bound = upper_bound;
    stmt->body = body;
    return stmt;
}

enum BoundNodeKind bound_for_statement_get_kind(struct BoundForStatement* stmt)
{
    (void)stmt;
    return BOUND_NODE_KIND_FOR_STATEMENT;
}

struct BoundNodeList* bound_for_statement_get_children(struct BoundForStatement* stmt)
{
    struct BoundNodeList* children = mc_malloc(sizeof(struct BoundNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct BoundNode*)stmt->lower_bound);
    LIST_PUSH(children, (struct BoundNode*)stmt->upper_bound);
    LIST_PUSH(children, (struct BoundNode*)stmt->body);
    return children;
}
