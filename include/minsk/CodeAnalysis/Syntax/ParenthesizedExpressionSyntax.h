#pragma once

#include "ExpressionSyntax.h"

struct ParenthesizedExpressionSyntax
{
    struct ExpressionSyntax base;
    struct SyntaxToken *open_parenthesis_token;
    struct ExpressionSyntax *expression;
    struct SyntaxToken *close_parenthesis_token;
};

struct ParenthesizedExpressionSyntax *parenthesized_expression_syntax_new(struct SyntaxToken *open_parenthesis_token,
                                                                          struct ExpressionSyntax *expression,
                                                                          struct SyntaxToken *close_parenthesis_token);
enum SyntaxKind parenthesized_expression_syntax_get_kind(struct ParenthesizedExpressionSyntax *syntax);
struct SyntaxNodeList *parenthesized_expression_syntax_get_children(struct ParenthesizedExpressionSyntax *syntax);
