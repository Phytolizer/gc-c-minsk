#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

struct BinaryExpressionSyntax
{
    struct ExpressionSyntax base;
    struct ExpressionSyntax *left;
    struct SyntaxToken *operator_token;
    struct ExpressionSyntax *right;
};

struct BinaryExpressionSyntax *binary_expression_syntax_new(struct ExpressionSyntax *left,
                                                            struct SyntaxToken *operator_token,
                                                            struct ExpressionSyntax *right);
enum SyntaxKind binary_expression_syntax_get_kind(struct BinaryExpressionSyntax *syntax);
struct SyntaxNodeList *binary_expression_syntax_get_children(struct BinaryExpressionSyntax *syntax);
