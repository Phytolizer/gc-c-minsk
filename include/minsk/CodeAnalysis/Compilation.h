#pragma once

#include <minsk/CodeAnalysis/VariableStore.h>

#include "EvaluationResult.h"
#include "Syntax/SyntaxTree.h"

struct BoundGlobalScope;

struct Compilation
{
    struct Compilation* previous;
    struct SyntaxTree* syntax;
    struct BoundGlobalScope* global_scope;
};

struct Compilation* compilation_new(struct SyntaxTree* syntax);
struct Compilation* compilation_continue_with(struct Compilation* compilation, struct SyntaxTree* syntax);
struct EvaluationResult* compilation_evaluate(struct Compilation* compilation, struct VariableStore* variables);
void compilation_emit_tree(struct Compilation* compilation, FILE* stream);
