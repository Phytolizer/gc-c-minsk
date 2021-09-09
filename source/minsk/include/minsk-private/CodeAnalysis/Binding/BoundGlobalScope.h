#pragma once

#include <common/List.h>
#include <minsk/CodeAnalysis/Diagnostic.h>

#include "BoundScope.h"
#include "BoundStatement.h"

struct BoundGlobalScope
{
    struct BoundGlobalScope* previous;
    struct DiagnosticList* diagnostics;
    struct VariableSymbolList* variables;
    struct BoundStatement* statement;
};

DECLARE_NAMED_LIST(BoundGlobalScopeList, struct BoundGlobalScope*);

struct BoundGlobalScope* bound_global_scope_new(
    struct BoundGlobalScope* previous,
    struct DiagnosticList* diagnostics,
    struct VariableSymbolList* variables,
    struct BoundStatement* statement);
