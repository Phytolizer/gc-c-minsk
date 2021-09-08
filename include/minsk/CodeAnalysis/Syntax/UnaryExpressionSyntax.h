#pragma once

#include "ExpressionSyntax.h"

struct UnaryExpressionSyntax
{
    struct ExpressionSyntax base;
    struct SyntaxToken* operator_token;
    struct ExpressionSyntax* operand;
};

struct UnaryExpressionSyntax* unary_expression_syntax_new(
    struct SyntaxToken* operator_token, struct ExpressionSyntax* operand);
enum SyntaxKind unary_expression_syntax_get_kind(struct UnaryExpressionSyntax* syntax);
struct SyntaxNodeList* unary_expression_syntax_get_children(struct UnaryExpressionSyntax* syntax);
