#pragma once

#include <minsk/CodeAnalysis/Syntax/ExpressionSyntax.h>

struct NameExpressionSyntax
{
    struct ExpressionSyntax base;
    struct SyntaxToken *identifier_token;
};

struct NameExpressionSyntax *name_expression_syntax_new(struct SyntaxToken *identifier_token);
enum SyntaxKind name_expression_syntax_get_kind(struct NameExpressionSyntax *syntax);
struct SyntaxNodeList *name_expression_syntax_get_children(struct NameExpressionSyntax *syntax);
