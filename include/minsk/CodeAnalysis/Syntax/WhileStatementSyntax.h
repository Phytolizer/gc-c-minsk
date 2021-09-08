#pragma once

#include "StatementSyntax.h"

struct WhileStatementSyntax
{
    struct StatementSyntax base;
    struct SyntaxToken* while_keyword;
    struct ExpressionSyntax* condition;
    struct StatementSyntax* body;
};

struct WhileStatementSyntax* while_statement_syntax_new(struct SyntaxToken* while_keyword,
                                                        struct ExpressionSyntax* condition,
                                                        struct StatementSyntax* body);
enum SyntaxKind while_statement_syntax_get_kind(struct WhileStatementSyntax* syntax);
struct SyntaxNodeList* while_statement_syntax_get_children(struct WhileStatementSyntax* syntax);
