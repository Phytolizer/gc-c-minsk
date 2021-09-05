#pragma once

#include <common/List.h>
#include <minsk/CodeAnalysis/DiagnosticBag.h>
#include <minsk/CodeAnalysis/Syntax/ExpressionSyntax.h>
#include <minsk/CodeAnalysis/VariableStore.h>

#include "BoundExpression.h"

struct Binder
{
  struct DiagnosticBag* diagnostics;
  struct VariableStore* variables;
};

struct Binder* binder_new(struct VariableStore* variables);
struct BoundExpression* binder_bind(
    struct Binder* binder,
    struct ExpressionSyntax* syntax);
