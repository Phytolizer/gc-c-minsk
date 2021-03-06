#include "minsk-private/CodeAnalysis/Binding/BoundBlockStatement.h"

struct BoundBlockStatement* bound_block_statement_new(struct BoundStatementList* statements)
{
    struct BoundBlockStatement* stmt = mc_malloc(sizeof(struct BoundBlockStatement));
    bound_statement_init(&stmt->base, BOUND_STATEMENT_KIND_BLOCK);
    stmt->statements = statements;
    return stmt;
}

enum BoundNodeKind bound_block_statement_get_kind(struct BoundBlockStatement* stmt)
{
    (void)stmt;
    return BOUND_NODE_KIND_BLOCK_STATEMENT;
}

struct BoundNodeList* bound_block_statement_get_children(struct BoundBlockStatement* stmt)
{
    struct BoundNodeList* children = mc_malloc(sizeof(struct BoundNodeList));
    LIST_INIT(children);
    for (long i = 0; i < stmt->statements->length; ++i)
    {
        LIST_PUSH(children, (struct BoundNode*)stmt->statements->data[i]);
    }
    return children;
}
