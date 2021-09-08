#pragma once

#include "ExpressionSyntax.h"

struct AssignmentExpressionSyntax
{
    struct ExpressionSyntax base;
    struct SyntaxToken* identifier_token;
    struct SyntaxToken* equals_token;
    struct ExpressionSyntax* expression;
};

struct AssignmentExpressionSyntax* assignment_expression_new(struct SyntaxToken* identifier_token,
                                                             struct SyntaxToken* equals_token,
                                                             struct ExpressionSyntax* expression);
enum SyntaxKind assignment_expression_syntax_get_kind(struct AssignmentExpressionSyntax* syntax);
struct SyntaxNodeList* assignment_expression_syntax_get_children(struct AssignmentExpressionSyntax* syntax);
