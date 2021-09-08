#include "minsk/CodeAnalysis/Syntax/WhileStatementSyntax.h"

struct WhileStatementSyntax *while_statement_syntax_new(struct SyntaxToken *while_keyword,
                                                        struct ExpressionSyntax *condition,
                                                        struct StatementSyntax *body)
{
    struct WhileStatementSyntax *syntax = mc_malloc(sizeof(struct WhileStatementSyntax));
    statement_syntax_init(&syntax->base, STATEMENT_SYNTAX_KIND_WHILE_STATEMENT_SYNTAX);
    syntax->while_keyword = while_keyword;
    syntax->condition = condition;
    syntax->body = body;
    return syntax;
}

enum SyntaxKind while_statement_syntax_get_kind(struct WhileStatementSyntax *syntax)
{
    (void)syntax;
    return SYNTAX_KIND_WHILE_STATEMENT;
}

struct SyntaxNodeList *while_statement_syntax_get_children(struct WhileStatementSyntax *syntax)
{
    struct SyntaxNodeList *children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode *)syntax->while_keyword);
    LIST_PUSH(children, (struct SyntaxNode *)syntax->condition);
    LIST_PUSH(children, (struct SyntaxNode *)syntax->body);
    return children;
}
