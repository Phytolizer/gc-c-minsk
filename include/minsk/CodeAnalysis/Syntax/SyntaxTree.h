#pragma once

#include <minsk/CodeAnalysis/DiagnosticBag.h>

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"
#include "common/List.h"

struct SyntaxTree
{
  struct DiagnosticBag* diagnostics;
  struct ExpressionSyntax* root;
  struct SyntaxToken* end_of_file_token;
};

struct SyntaxTree* syntax_tree_new(
    struct DiagnosticBag* diagnostics,
    struct ExpressionSyntax* root,
    struct SyntaxToken* end_of_file_token);
struct SyntaxTree* syntax_tree_parse(sds text);
