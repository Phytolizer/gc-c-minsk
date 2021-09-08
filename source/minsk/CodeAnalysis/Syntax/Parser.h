#pragma once

#include <common/List.h>
#include <minsk/CodeAnalysis/DiagnosticBag.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <minsk/CodeAnalysis/Text/SourceText.h>
#include <sds.h>

struct Parser
{
    struct SourceText* source_text;
    struct SyntaxTokenList* tokens;
    int position;
    struct DiagnosticBag* diagnostics;
};

struct Parser* parser_new(struct SourceText* source_text);
void parser_free(struct Parser* parser);
struct CompilationUnitSyntax* parser_parse_compilation_unit(struct Parser* parser);
