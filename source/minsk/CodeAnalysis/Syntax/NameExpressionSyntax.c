#include "minsk/CodeAnalysis/Syntax/NameExpressionSyntax.h"

struct NameExpressionSyntax *name_expression_syntax_new(struct SyntaxToken *identifier_token)
{
    struct NameExpressionSyntax *syntax = mc_malloc(sizeof(struct NameExpressionSyntax));
    expression_syntax_init((struct ExpressionSyntax *)syntax, EXPRESSION_SYNTAX_KIND_NAME_EXPRESSION_SYNTAX);
    syntax->identifier_token = identifier_token;
    return syntax;
}

enum SyntaxKind name_expression_syntax_get_kind(struct NameExpressionSyntax *syntax)
{
    (void)syntax;
    return SYNTAX_KIND_NAME_EXPRESSION;
}

struct SyntaxNodeList *name_expression_syntax_get_children(struct NameExpressionSyntax *syntax)
{
    struct SyntaxNodeList *children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode *)syntax->identifier_token);
    return children;
}
