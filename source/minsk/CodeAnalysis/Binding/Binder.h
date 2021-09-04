#pragma once

#include <common/List.h>
#include <minsk/CodeAnalysis/Syntax/ExpressionSyntax.h>

#include "BoundExpression.h"

struct Binder
{
  struct StringList* diagnostics;
};

struct Binder* binder_new(void);
struct BoundExpression* binder_bind(
    struct Binder* binder,
    struct ExpressionSyntax* syntax);