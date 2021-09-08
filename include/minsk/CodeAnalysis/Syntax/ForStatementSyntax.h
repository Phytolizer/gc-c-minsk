#pragma once

#include "StatementSyntax.h"

struct ForStatementSyntax
{
    struct StatementSyntax base;
    struct SyntaxToken* for_keyword;
    struct SyntaxToken* identifier_token;
    struct SyntaxToken* equals_token;
    struct ExpressionSyntax* lower_bound;
    struct SyntaxToken* to_keyword;
    struct ExpressionSyntax* upper_bound;
    struct StatementSyntax* body;
};

struct ForStatementSyntax* for_statement_syntax_new(
    struct SyntaxToken* for_keyword,
    struct SyntaxToken* identifier_token,
    struct SyntaxToken* equals_token,
    struct ExpressionSyntax* lower_bound,
    struct SyntaxToken* to_keyword,
    struct ExpressionSyntax* upper_bound,
    struct StatementSyntax* body);
enum SyntaxKind for_statement_syntax_get_kind(struct ForStatementSyntax* syntax);
struct SyntaxNodeList* for_statement_syntax_get_children(struct ForStatementSyntax* syntax);
