#include "minsk-private/CodeAnalysis/Binding/BoundWhileStatement.h"

struct BoundWhileStatement* bound_while_statement_new(struct BoundExpression* condition, struct BoundStatement* body)
{
    struct BoundWhileStatement* result = mc_malloc(sizeof(struct BoundWhileStatement));
    bound_statement_init(&result->base, BOUND_STATEMENT_KIND_WHILE);
    result->condition = condition;
    result->body = body;
    return result;
}

enum BoundNodeKind bound_while_statement_get_kind(struct BoundWhileStatement* stmt)
{
    (void)stmt;
    return BOUND_NODE_KIND_WHILE_STATEMENT;
}

struct BoundNodeList* bound_while_statement_get_children(struct BoundWhileStatement* stmt)
{
    struct BoundNodeList* children = mc_malloc(sizeof(struct BoundNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct BoundNode*)stmt->condition);
    LIST_PUSH(children, (struct BoundNode*)stmt->body);
    return children;
}
