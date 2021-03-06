#pragma once

#include <common/List.h>
#include <stdio.h>

#define BOUND_NODE_TYPES_ \
    X(EXPRESSION) \
    X(STATEMENT)

enum BoundNodeType
{
#define X(x) BOUND_NODE_TYPE_##x,
    BOUND_NODE_TYPES_
#undef X
};

#define BOUND_NODE_KINDS_ \
    X(BINARY_EXPRESSION) \
    X(LITERAL_EXPRESSION) \
    X(UNARY_EXPRESSION) \
    X(VARIABLE_EXPRESSION) \
    X(ASSIGNMENT_EXPRESSION) \
    X(BLOCK_STATEMENT) \
    X(EXPRESSION_STATEMENT) \
    X(FOR_STATEMENT) \
    X(IF_STATEMENT) \
    X(VARIABLE_DECLARATION) \
    X(WHILE_STATEMENT)

enum BoundNodeKind
{
#define X(x) BOUND_NODE_KIND_##x,
    BOUND_NODE_KINDS_
#undef X
};

extern const char* const BOUND_NODE_KINDS[];

struct BoundNode
{
    enum BoundNodeType type;
};

DECLARE_NAMED_LIST(BoundNodeList, struct BoundNode*);

enum BoundNodeKind bound_node_get_kind(struct BoundNode* node);
struct BoundNodeList* bound_node_get_children(struct BoundNode* node);
void bound_node_pretty_print(FILE* stream, struct BoundNode* node);
