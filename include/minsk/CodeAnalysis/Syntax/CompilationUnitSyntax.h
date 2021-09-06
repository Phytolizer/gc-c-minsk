#pragma once

#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>

#include "SyntaxNode.h"

struct CompilationUnitSyntax
{
  struct SyntaxNode base;
  struct ExpressionSyntax* expression;
  struct SyntaxToken* end_of_file_token;
};

struct CompilationUnitSyntax* compilation_unit_syntax_new(
    struct ExpressionSyntax* expression,
    struct SyntaxToken* end_of_file_token);
enum SyntaxKind compilation_unit_syntax_get_kind(struct CompilationUnitSyntax* unit);
struct SyntaxNodeList* compilation_unit_syntax_get_children(
    struct CompilationUnitSyntax* unit);
