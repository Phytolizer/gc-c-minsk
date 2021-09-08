#include "minsk/CodeAnalysis/Syntax/ForStatementSyntax.h"

struct ForStatementSyntax* for_statement_syntax_new(
    struct SyntaxToken* for_keyword,
    struct SyntaxToken* identifier_token,
    struct SyntaxToken* equals_token,
    struct ExpressionSyntax* lower_bound,
    struct SyntaxToken* to_keyword,
    struct ExpressionSyntax* upper_bound,
    struct StatementSyntax* body)
{
    struct ForStatementSyntax* syntax = mc_malloc(sizeof(struct ForStatementSyntax));
    statement_syntax_init(&syntax->base, STATEMENT_SYNTAX_KIND_FOR_STATEMENT_SYNTAX);
    syntax->for_keyword = for_keyword;
    syntax->identifier_token = identifier_token;
    syntax->equals_token = equals_token;
    syntax->lower_bound = lower_bound;
    syntax->to_keyword = to_keyword;
    syntax->upper_bound = upper_bound;
    syntax->body = body;
    return syntax;
}

enum SyntaxKind for_statement_syntax_get_kind(struct ForStatementSyntax* syntax)
{
    (void)syntax;
    return SYNTAX_KIND_FOR_STATEMENT;
}

struct SyntaxNodeList* for_statement_syntax_get_children(struct ForStatementSyntax* syntax)
{
    struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->for_keyword);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->identifier_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->equals_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->lower_bound);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->upper_bound);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->body);
    return children;
}
