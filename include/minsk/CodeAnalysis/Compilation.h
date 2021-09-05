#pragma once

#include <minsk/CodeAnalysis/VariableStore.h>

#include "EvaluationResult.h"
#include "Syntax/SyntaxTree.h"

struct Compilation
{
  struct SyntaxTree* syntax;
};

struct Compilation* compilation_new(struct SyntaxTree* syntax);
struct EvaluationResult* compilation_evaluate(
    struct Compilation* compilation,
    struct VariableStore* variables);
