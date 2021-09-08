#include "BoundVariableDeclaration.h"

#include "BoundStatement.h"

struct BoundVariableDeclaration *bound_variable_declaration_new(struct VariableSymbol *variable,
                                                                struct BoundExpression *initializer)
{
    struct BoundVariableDeclaration *decl = mc_malloc(sizeof(struct BoundVariableDeclaration));
    bound_statement_init(&decl->base, BOUND_STATEMENT_KIND_VARIABLE_DECLARATION);
    decl->variable = variable;
    decl->initializer = initializer;
    return decl;
}

enum BoundNodeKind bound_variable_declaration_get_kind(struct BoundVariableDeclaration *decl)
{
    (void)decl;
    return BOUND_NODE_KIND_VARIABLE_DECLARATION;
}
