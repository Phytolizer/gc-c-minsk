#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"

struct ExpressionStatementSyntax
{
    struct StatementSyntax base;
    struct ExpressionSyntax* expression;
};

struct ExpressionStatementSyntax* expression_statement_syntax_new(struct ExpressionSyntax* expression);
enum SyntaxKind expression_statement_syntax_get_kind(struct ExpressionStatementSyntax* syntax);
struct SyntaxNodeList* expression_statement_syntax_get_children(struct ExpressionStatementSyntax* syntax);
