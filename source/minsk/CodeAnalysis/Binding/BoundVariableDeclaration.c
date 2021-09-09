#include "minsk-private/CodeAnalysis/Binding/BoundVariableDeclaration.h"

struct BoundVariableDeclaration* bound_variable_declaration_new(
    struct VariableSymbol* variable, struct BoundExpression* initializer)
{
    struct BoundVariableDeclaration* decl = mc_malloc(sizeof(struct BoundVariableDeclaration));
    bound_statement_init(&decl->base, BOUND_STATEMENT_KIND_VARIABLE_DECLARATION);
    decl->variable = variable;
    decl->initializer = initializer;
    return decl;
}

enum BoundNodeKind bound_variable_declaration_get_kind(struct BoundVariableDeclaration* decl)
{
    (void)decl;
    return BOUND_NODE_KIND_VARIABLE_DECLARATION;
}

struct BoundNodeList* bound_variable_declaration_get_children(struct BoundVariableDeclaration* decl)
{
    struct BoundNodeList* children = mc_malloc(sizeof(struct BoundNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct BoundNode*)decl->initializer);
    return children;
}
