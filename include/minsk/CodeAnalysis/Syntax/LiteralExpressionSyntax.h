#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

struct LiteralExpressionSyntax
{
    struct ExpressionSyntax base;
    struct SyntaxToken *literal_token;
    struct Object *value;
};

struct LiteralExpressionSyntax *literal_expression_syntax_new(struct SyntaxToken *number_token, struct Object *value);
enum SyntaxKind literal_expression_syntax_get_kind(struct LiteralExpressionSyntax *syntax);
struct SyntaxNodeList *literal_expression_syntax_get_children(struct LiteralExpressionSyntax *syntax);
