#pragma once

#include "ElseClauseSyntax.h"
#include "ExpressionSyntax.h"
#include "StatementSyntax.h"

struct IfStatementSyntax
{
  struct StatementSyntax base;
  struct SyntaxToken* if_keyword;
  struct ExpressionSyntax* condition;
  struct StatementSyntax* then_statement;
  struct ElseClauseSyntax* else_clause;
};

struct IfStatementSyntax* if_statement_syntax_new(
    struct SyntaxToken* if_keyword,
    struct ExpressionSyntax* condition,
    struct StatementSyntax* then_statement,
    struct ElseClauseSyntax* else_clause);
enum SyntaxKind if_statement_syntax_get_kind(struct IfStatementSyntax* syntax);
struct SyntaxNodeList* if_statement_syntax_get_children(
    struct IfStatementSyntax* syntax);
