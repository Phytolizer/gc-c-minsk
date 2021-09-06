#pragma once

#include <common/List.h>
#include <minsk/CodeAnalysis/DiagnosticBag.h>
#include <minsk/CodeAnalysis/Text/SourceText.h>

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

struct SyntaxTree
{
  struct SourceText* source_text;
  struct DiagnosticBag* diagnostics;
  struct ExpressionSyntax* root;
  struct SyntaxToken* end_of_file_token;
};

struct SyntaxTree* syntax_tree_new(
    struct SourceText* source_text,
    struct DiagnosticBag* diagnostics,
    struct ExpressionSyntax* root,
    struct SyntaxToken* end_of_file_token);
struct SyntaxTree* syntax_tree_parse(sds text);
struct SyntaxTree* syntax_tree_parse_text(struct SourceText* source_text);
struct SyntaxTokenList* syntax_tree_parse_tokens(sds text);
struct SyntaxTokenList* syntax_tree_parse_text_tokens(
    struct SourceText* source_text);
