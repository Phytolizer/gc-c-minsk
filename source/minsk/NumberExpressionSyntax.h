#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

struct NumberExpressionSyntax
{
  struct ExpressionSyntax base;
  struct SyntaxToken* number_token;
};

struct NumberExpressionSyntax* number_expression_syntax_new(
    struct SyntaxToken* number_token);
void number_expression_syntax_free(struct NumberExpressionSyntax* syntax);
enum SyntaxKind number_expression_syntax_get_kind(
    struct NumberExpressionSyntax* syntax);
struct SyntaxNodeList* number_expression_syntax_get_children(
    struct NumberExpressionSyntax* syntax);
