#include "minsk/CodeAnalysis/Syntax/IfStatementSyntax.h"

struct IfStatementSyntax* if_statement_syntax_new(
    struct SyntaxToken* if_keyword,
    struct ExpressionSyntax* condition,
    struct StatementSyntax* then_statement,
    struct ElseClauseSyntax* else_clause)
{
    struct IfStatementSyntax* syntax = mc_malloc(sizeof(struct IfStatementSyntax));
    statement_syntax_init(&syntax->base, STATEMENT_SYNTAX_KIND_IF_STATEMENT_SYNTAX);
    syntax->if_keyword = if_keyword;
    syntax->condition = condition;
    syntax->then_statement = then_statement;
    syntax->else_clause = else_clause;
    return syntax;
}

enum SyntaxKind if_statement_syntax_get_kind(struct IfStatementSyntax* syntax)
{
    (void)syntax;
    return SYNTAX_KIND_IF_STATEMENT;
}

struct SyntaxNodeList* if_statement_syntax_get_children(struct IfStatementSyntax* syntax)
{
    struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->if_keyword);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->condition);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->then_statement);
    if (syntax->else_clause)
    {
        LIST_PUSH(children, (struct SyntaxNode*)syntax->else_clause);
    }
    return children;
}
