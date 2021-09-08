#include "minsk/CodeAnalysis/Syntax/VariableDeclarationSyntax.h"

#include "minsk/CodeAnalysis/Syntax/StatementSyntax.h"

struct VariableDeclarationSyntax* variable_declaration_syntax_new(struct SyntaxToken* keyword_token,
                                                                  struct SyntaxToken* identifier_token,
                                                                  struct SyntaxToken* equals_token,
                                                                  struct ExpressionSyntax* initializer)
{
    struct VariableDeclarationSyntax* syntax = mc_malloc(sizeof(struct VariableDeclarationSyntax));
    statement_syntax_init(&syntax->base, STATEMENT_SYNTAX_KIND_VARIABLE_DECLARATION_SYNTAX);
    syntax->keyword_token = keyword_token;
    syntax->identifier_token = identifier_token;
    syntax->equals_token = equals_token;
    syntax->initializer = initializer;
    return syntax;
}

enum SyntaxKind variable_declaration_syntax_get_kind(struct VariableDeclarationSyntax* syntax)
{
    (void)syntax;
    return SYNTAX_KIND_VARIABLE_DECLARATION;
}

struct SyntaxNodeList* variable_declaration_syntax_get_children(struct VariableDeclarationSyntax* syntax)
{
    struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->keyword_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->identifier_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->equals_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->initializer);
    return children;
}
