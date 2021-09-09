#pragma once

#include "BoundStatement.h"

struct BoundForStatement
{
    struct BoundStatement base;
    struct VariableSymbol* variable;
    struct BoundExpression* lower_bound;
    struct BoundExpression* upper_bound;
    struct BoundStatement* body;
};

struct BoundForStatement* bound_for_statement_new(
    struct VariableSymbol* variable,
    struct BoundExpression* lower_bound,
    struct BoundExpression* upper_bound,
    struct BoundStatement* body);
enum BoundNodeKind bound_for_statement_get_kind(struct BoundForStatement* stmt);
struct BoundNodeList* bound_for_statement_get_children(struct BoundForStatement* stmt);
