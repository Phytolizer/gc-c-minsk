#pragma once

#include "BoundExpression.h"
#include "BoundStatement.h"

struct BoundIfStatement
{
    struct BoundStatement base;
    struct BoundExpression *condition;
    struct BoundStatement *then_statement;
    struct BoundStatement *else_statement;
};

struct BoundIfStatement *bound_if_statement_new(struct BoundExpression *condition,
                                                struct BoundStatement *then_statement,
                                                struct BoundStatement *else_statement);
enum BoundNodeKind bound_if_statement_get_kind(struct BoundIfStatement *stmt);
