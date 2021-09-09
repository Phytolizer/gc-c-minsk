#pragma once

#include <common/List.h>

#include "BoundNode.h"

#define BOUND_STATEMENT_KINDS_ \
    X(BLOCK) \
    X(EXPRESSION) \
    X(FOR) \
    X(IF) \
    X(VARIABLE_DECLARATION) \
    X(WHILE)

enum BoundStatementKind
{
#define X(x) BOUND_STATEMENT_KIND_##x,
    BOUND_STATEMENT_KINDS_
#undef X
};

struct BoundStatement
{
    struct BoundNode base;
    enum BoundStatementKind kind;
};

DECLARE_NAMED_LIST(BoundStatementList, struct BoundStatement*);

void bound_statement_init(struct BoundStatement* stmt, enum BoundStatementKind kind);
enum BoundNodeKind bound_statement_get_kind(struct BoundStatement* stmt);
struct BoundNodeList* bound_statement_get_children(struct BoundStatement* stmt);
