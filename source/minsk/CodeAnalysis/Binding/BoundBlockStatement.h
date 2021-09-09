#pragma once

#include "BoundStatement.h"

struct BoundBlockStatement
{
    struct BoundStatement base;
    struct BoundStatementList* statements;
};

struct BoundBlockStatement* bound_block_statement_new(struct BoundStatementList* statements);
enum BoundNodeKind bound_block_statement_get_kind(struct BoundBlockStatement* stmt);
struct BoundNodeList* bound_block_statement_get_children(struct BoundBlockStatement* stmt);
