#include "minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h"

#include <IncludeMe.h>

struct BinaryExpressionSyntax* binary_expression_syntax_new(
    struct ExpressionSyntax* left, struct SyntaxToken* operator_token, struct ExpressionSyntax* right)
{
    struct BinaryExpressionSyntax* syntax = mc_malloc(sizeof(struct BinaryExpressionSyntax));
    expression_syntax_init((struct ExpressionSyntax*)syntax, EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX);
    syntax->left = left;
    syntax->operator_token = operator_token;
    syntax->right = right;
    return syntax;
}

enum SyntaxKind binary_expression_syntax_get_kind(struct BinaryExpressionSyntax* syntax)
{
    (void)syntax;
    return SYNTAX_KIND_BINARY_EXPRESSION;
}

struct SyntaxNodeList* binary_expression_syntax_get_children(struct BinaryExpressionSyntax* syntax)
{
    struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->left);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->operator_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->right);
    return children;
}
