#include "minsk/CodeAnalysis/Compilation.h"

#include <minsk/CodeAnalysis/EvaluationResult.h>
#include <minsk/CodeAnalysis/Syntax/CompilationUnitSyntax.h>

#include "Binding/Binder.h"
#include "Evaluator.h"

struct Compilation* compilation_new(struct SyntaxTree* syntax)
{
  struct Compilation* result = mc_malloc(sizeof(struct Compilation));
  result->syntax = syntax;
  return result;
}

struct EvaluationResult* compilation_evaluate(
    struct Compilation* compilation,
    struct VariableStore* variables)
{
  struct Binder* binder = binder_new(variables);
  struct BoundExpression* bound_expression
      = binder_bind(binder, compilation->syntax->root->expression);
  struct DiagnosticList* diagnostics
      = compilation->syntax->diagnostics->diagnostics;
  for (long i = 0; i < binder->diagnostics->diagnostics->length; i++)
  {
    LIST_PUSH(diagnostics, binder->diagnostics->diagnostics->data[i]);
  }
  if (diagnostics->length > 0)
  {
    return evaluation_result_new(diagnostics, OBJECT_NULL());
  }
  struct Evaluator* evaluator = evaluator_new(bound_expression, variables);
  struct Object* value = evaluator_evaluate(evaluator);
  return evaluation_result_new(diagnostics, value);
}
