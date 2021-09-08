#include "minsk/CodeAnalysis/Syntax/BlockStatementSyntax.h"

struct BlockStatementSyntax *block_statement_syntax_new(struct SyntaxToken *open_brace_token,
                                                        struct StatementSyntaxList *statements,
                                                        struct SyntaxToken *close_brace_token)
{
    struct BlockStatementSyntax *syntax = mc_malloc(sizeof(struct BlockStatementSyntax));
    statement_syntax_init((struct StatementSyntax *)syntax, STATEMENT_SYNTAX_KIND_BLOCK_STATEMENT_SYNTAX);
    syntax->open_brace_token = open_brace_token;
    syntax->statements = statements;
    syntax->close_brace_token = close_brace_token;
    return syntax;
}

enum SyntaxKind block_statement_syntax_get_kind(struct BlockStatementSyntax *syntax)
{
    (void)syntax;
    return SYNTAX_KIND_BLOCK_STATEMENT;
}

struct SyntaxNodeList *block_statement_syntax_get_children(struct BlockStatementSyntax *syntax)
{
    struct SyntaxNodeList *children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode *)syntax->open_brace_token);
    for (long i = 0; i < syntax->statements->length; ++i)
    {
        LIST_PUSH(children, (struct SyntaxNode *)syntax->statements->data[i]);
    }
    LIST_PUSH(children, (struct SyntaxNode *)syntax->close_brace_token);
    return children;
}
