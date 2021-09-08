#include "BoundNode.h"

#include "BoundExpression.h"
#include "BoundStatement.h"

const char* const BOUND_NODE_KINDS[] = {
#define X(x) #x,
    BOUND_NODE_KINDS_
#undef X
};

enum BoundNodeKind bound_node_get_kind(struct BoundNode* node)
{
    switch (node->type)
    {
    case BOUND_NODE_TYPE_EXPRESSION:
        return bound_expression_get_kind((struct BoundExpression*)node);
    case BOUND_NODE_TYPE_STATEMENT:
        return bound_statement_get_kind((struct BoundStatement*)node);
    }
}
