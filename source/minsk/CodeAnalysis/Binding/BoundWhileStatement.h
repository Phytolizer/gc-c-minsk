#pragma once

#include "BoundExpression.h"
#include "BoundStatement.h"

struct BoundWhileStatement
{
    struct BoundStatement base;
    struct BoundExpression *condition;
    struct BoundStatement *body;
};

struct BoundWhileStatement *bound_while_statement_new(struct BoundExpression *condition, struct BoundStatement *body);
enum BoundNodeKind bound_while_statement_get_kind(struct BoundWhileStatement *stmt);
