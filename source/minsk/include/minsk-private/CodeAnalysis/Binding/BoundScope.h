#pragma once

#include <minsk/CodeAnalysis/VariableSymbol.h>
#include <sds.h>

struct BoundScopeVariableEntry
{
    sds name;
    struct VariableSymbol* value;
};

struct BoundScopeVariables
{
    struct BoundScopeVariableEntry* entries;
    long count;
    long num_entries;
};

struct BoundScope
{
    struct BoundScopeVariables* variables;
    struct BoundScope* parent;
};

struct BoundScope* bound_scope_new(struct BoundScope* parent);
struct VariableSymbol** bound_scope_try_lookup(struct BoundScope* scope, sds name);
bool bound_scope_try_declare(struct BoundScope* scope, struct VariableSymbol* variable);
struct VariableSymbolList* bound_scope_get_declared_variables(struct BoundScope* scope);
