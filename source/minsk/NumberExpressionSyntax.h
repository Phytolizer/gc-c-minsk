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
enum SyntaxKind number_expression_syntax_get_kind(
    struct NumberExpressionSyntax* syntax);
