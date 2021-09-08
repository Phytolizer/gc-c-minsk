#include "minsk/CodeAnalysis/Syntax/ExpressionStatementSyntax.h"

struct ExpressionStatementSyntax* expression_statement_syntax_new(struct ExpressionSyntax* expression)
{
    struct ExpressionStatementSyntax* syntax = mc_malloc(sizeof(struct ExpressionStatementSyntax));
    statement_syntax_init((struct StatementSyntax*)syntax, STATEMENT_SYNTAX_KIND_EXPRESSION_STATEMENT_SYNTAX);
    syntax->expression = expression;
    return syntax;
}

enum SyntaxKind expression_statement_syntax_get_kind(struct ExpressionStatementSyntax* syntax)
{
    (void)syntax;
    return SYNTAX_KIND_EXPRESSION_STATEMENT;
}

struct SyntaxNodeList* expression_statement_syntax_get_children(struct ExpressionStatementSyntax* syntax)
{
    struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->expression);
    return children;
}
