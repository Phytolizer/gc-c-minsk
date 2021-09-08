#pragma once

#include "StatementSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"

struct CompilationUnitSyntax
{
    struct SyntaxNode base;
    struct StatementSyntax *statement;
    struct SyntaxToken *end_of_file_token;
};

struct CompilationUnitSyntax *compilation_unit_syntax_new(struct StatementSyntax *statement,
                                                          struct SyntaxToken *end_of_file_token);
enum SyntaxKind compilation_unit_syntax_get_kind(struct CompilationUnitSyntax *unit);
struct SyntaxNodeList *compilation_unit_syntax_get_children(struct CompilationUnitSyntax *unit);
