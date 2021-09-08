#pragma once

#include "SyntaxKind.h"
#include "SyntaxNode.h"

struct ElseClauseSyntax
{
    struct SyntaxNode base;
    struct SyntaxToken *else_keyword;
    struct StatementSyntax *else_statement;
};

struct ElseClauseSyntax *else_clause_syntax_new(struct SyntaxToken *else_keyword,
                                                struct StatementSyntax *else_statement);
enum SyntaxKind else_clause_syntax_get_kind(struct ElseClauseSyntax *syntax);
struct SyntaxNodeList *else_clause_syntax_get_children(struct ElseClauseSyntax *syntax);
