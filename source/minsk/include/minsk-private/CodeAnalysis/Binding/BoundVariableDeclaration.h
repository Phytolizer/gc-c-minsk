#pragma once

#include <minsk/CodeAnalysis/VariableSymbol.h>

#include "BoundStatement.h"

struct BoundVariableDeclaration
{
    struct BoundStatement base;
    struct VariableSymbol* variable;
    struct BoundExpression* initializer;
};

struct BoundVariableDeclaration* bound_variable_declaration_new(
    struct VariableSymbol* variable, struct BoundExpression* initializer);
enum BoundNodeKind bound_variable_declaration_get_kind(struct BoundVariableDeclaration* decl);
struct BoundNodeList* bound_variable_declaration_get_children(struct BoundVariableDeclaration* decl);
