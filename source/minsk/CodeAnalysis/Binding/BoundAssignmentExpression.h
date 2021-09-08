#pragma once

#include <minsk/CodeAnalysis/VariableSymbol.h>
#include <sds.h>

#include "BoundExpression.h"

struct BoundAssignmentExpression
{
    struct BoundExpression base;
    struct VariableSymbol* variable;
    struct BoundExpression* expression;
};

struct BoundAssignmentExpression* bound_assignment_expression_new(struct VariableSymbol* name,
                                                                  struct BoundExpression* expression);
enum BoundNodeKind bound_assignment_expression_get_kind(struct BoundAssignmentExpression* expression);
enum ObjectKind bound_assignment_expression_get_type(struct BoundAssignmentExpression* expression);
