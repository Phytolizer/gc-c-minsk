#include "minsk/CodeAnalysis/Syntax/ElseClauseSyntax.h"

struct ElseClauseSyntax* else_clause_syntax_new(
    struct SyntaxToken* else_keyword, struct StatementSyntax* else_statement)
{
    struct ElseClauseSyntax* syntax = mc_malloc(sizeof(struct ElseClauseSyntax));
    syntax->base.kind = SYNTAX_NODE_KIND_ELSE_CLAUSE;
    syntax->else_keyword = else_keyword;
    syntax->else_statement = else_statement;
    return syntax;
}

enum SyntaxKind else_clause_syntax_get_kind(struct ElseClauseSyntax* syntax)
{
    (void)syntax;
    return SYNTAX_KIND_ELSE_CLAUSE;
}

struct SyntaxNodeList* else_clause_syntax_get_children(struct ElseClauseSyntax* syntax)
{
    struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->else_keyword);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->else_statement);
    return children;
}
