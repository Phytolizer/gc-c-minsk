#pragma once

#include <common/List.h>
#include <minsk/CodeAnalysis/DiagnosticBag.h>
#include <minsk/CodeAnalysis/Syntax/CompilationUnitSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/StatementSyntax.h>
#include <minsk/CodeAnalysis/VariableStore.h>

#include "BoundExpression.h"
#include "BoundGlobalScope.h"
#include "BoundScope.h"

struct Binder
{
    struct DiagnosticBag* diagnostics;
    struct BoundScope* scope;
};

struct BoundGlobalScope* bind_global_scope(struct BoundGlobalScope* previous, struct CompilationUnitSyntax* syntax);
struct Binder* binder_new(struct BoundScope* parent);
