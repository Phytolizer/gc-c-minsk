#pragma once

#include "SyntaxNode.h"

#define EXPRESSION_SYNTAX_KINDS_ \
  X(LITERAL_EXPRESSION_SYNTAX) \
  X(BINARY_EXPRESSION_SYNTAX) \
  X(PARENTHESIZED_EXPRESSION_SYNTAX) \
  X(UNARY_EXPRESSION_SYNTAX) \
  X(NAME_EXPRESSION_SYNTAX) \
  X(ASSIGNMENT_EXPRESSION_SYNTAX)

enum ExpressionSyntaxKind
{
#define X(x) EXPRESSION_SYNTAX_KIND_##x,
  EXPRESSION_SYNTAX_KINDS_
#undef X
};

struct ExpressionSyntax
{
  struct SyntaxNode base;
  enum ExpressionSyntaxKind kind;
};

void expression_syntax_init(
    struct ExpressionSyntax* syntax,
    enum ExpressionSyntaxKind kind);
enum SyntaxKind expression_syntax_get_kind(struct ExpressionSyntax* syntax);
struct SyntaxNodeList* expression_syntax_get_children(
    struct ExpressionSyntax* syntax);
