#pragma once

#include "BoundExpression.h"
#include "BoundStatement.h"

struct BoundExpressionStatement
{
  struct BoundStatement base;
  struct BoundExpression* expression;
};

struct BoundExpressionStatement* bound_expression_statement_new(
    struct BoundExpression* expression);
enum BoundNodeKind bound_expression_statement_get_kind(
    struct BoundExpressionStatement* stmt);
