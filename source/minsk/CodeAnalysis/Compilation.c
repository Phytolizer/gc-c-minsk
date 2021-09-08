#include "Compilation.h"

#include <minsk/CodeAnalysis/EvaluationResult.h>
#include <minsk/CodeAnalysis/Syntax/CompilationUnitSyntax.h>

#include "Binding/Binder.h"
#include "Evaluator.h"

static struct Compilation *compilation_new_continued(struct Compilation *previous, struct SyntaxTree *syntax_tree);

struct Compilation *compilation_new(struct SyntaxTree *syntax)
{
    struct Compilation *result = mc_malloc(sizeof(struct Compilation));
    result->previous = NULL;
    result->syntax = syntax;
    result->global_scope = NULL;
    return result;
}

struct Compilation *compilation_continue_with(struct Compilation *compilation, struct SyntaxTree *syntax)
{
    return compilation_new_continued(compilation, syntax);
}

struct EvaluationResult *compilation_evaluate(struct Compilation *compilation, struct VariableStore *variables)
{
    struct BoundGlobalScope *global_scope = compilation_get_global_scope(compilation);
    struct BoundStatement *bound_statement = global_scope->statement;
    struct DiagnosticList *diagnostics = mc_malloc(sizeof(struct DiagnosticList));
    LIST_INIT(diagnostics);
    for (long i = 0; i < compilation->syntax->diagnostics->diagnostics->length; ++i)
    {
        LIST_PUSH(diagnostics, compilation->syntax->diagnostics->diagnostics->data[i]);
    }
    for (long i = 0; i < global_scope->diagnostics->length; ++i)
    {
        LIST_PUSH(diagnostics, global_scope->diagnostics->data[i]);
    }
    if (diagnostics->length > 0)
    {
        return evaluation_result_new(diagnostics, OBJECT_NULL());
    }
    struct Evaluator *evaluator = evaluator_new(bound_statement, variables);
    struct Object *value = evaluator_evaluate(evaluator);
    return evaluation_result_new(diagnostics, value);
}

struct BoundGlobalScope *compilation_get_global_scope(struct Compilation *compilation)
{
    if (compilation->global_scope == NULL)
    {
        struct BoundGlobalScope *previous_global_scope = NULL;
        if (compilation->previous)
        {
            previous_global_scope = compilation_get_global_scope(compilation->previous);
        }
        compilation->global_scope = bind_global_scope(previous_global_scope, compilation->syntax->root);
    }
    return compilation->global_scope;
}

static struct Compilation *compilation_new_continued(struct Compilation *previous, struct SyntaxTree *syntax_tree)
{
    struct Compilation *compilation = mc_malloc(sizeof(struct Compilation));
    compilation->previous = previous;
    compilation->syntax = syntax_tree;
    compilation->global_scope = NULL;
    return compilation;
}
