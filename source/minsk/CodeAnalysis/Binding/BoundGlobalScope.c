#include "BoundGlobalScope.h"

struct BoundGlobalScope* bound_global_scope_new(
    struct BoundGlobalScope* previous,
    struct DiagnosticList* diagnostics,
    struct VariableSymbolList* variables,
    struct BoundExpression* expression)
{
  struct BoundGlobalScope* scope = mc_malloc(sizeof(struct BoundGlobalScope));
  scope->previous = previous;
  scope->diagnostics = diagnostics;
  scope->variables = variables;
  scope->expression = expression;
  return scope;
}
