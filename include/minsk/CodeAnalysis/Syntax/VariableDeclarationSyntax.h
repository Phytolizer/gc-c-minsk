#pragma once

#include "StatementSyntax.h"
#include "SyntaxToken.h"

struct VariableDeclarationSyntax
{
    struct StatementSyntax base;
    struct SyntaxToken* keyword_token;
    struct SyntaxToken* identifier_token;
    struct SyntaxToken* equals_token;
    struct ExpressionSyntax* initializer;
};

struct VariableDeclarationSyntax* variable_declaration_syntax_new(struct SyntaxToken* keyword_token,
                                                                  struct SyntaxToken* identifier_token,
                                                                  struct SyntaxToken* equals_token,
                                                                  struct ExpressionSyntax* initializer);
enum SyntaxKind variable_declaration_syntax_get_kind(struct VariableDeclarationSyntax* syntax);
struct SyntaxNodeList* variable_declaration_syntax_get_children(struct VariableDeclarationSyntax* syntax);
