#pragma once

#include <minsk/CodeAnalysis/VariableSymbol.h>
#include <sds.h>

#include "BoundExpression.h"

struct BoundVariableExpression
{
    struct BoundExpression base;
    struct VariableSymbol* variable;
};

struct BoundVariableExpression* bound_variable_expression_new(struct VariableSymbol* variable);
enum BoundNodeKind bound_variable_expression_get_kind(struct BoundVariableExpression* expression);
enum ObjectKind bound_variable_expression_get_type(struct BoundVariableExpression* expression);
