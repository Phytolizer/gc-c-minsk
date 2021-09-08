#include "BoundGlobalScope.h"

struct BoundGlobalScope* bound_global_scope_new(struct BoundGlobalScope* previous, struct DiagnosticList* diagnostics,
                                                struct VariableSymbolList* variables, struct BoundStatement* statement)
{
    struct BoundGlobalScope* scope = mc_malloc(sizeof(struct BoundGlobalScope));
    scope->previous = previous;
    scope->diagnostics = diagnostics;
    scope->variables = variables;
    scope->statement = statement;
    return scope;
}
