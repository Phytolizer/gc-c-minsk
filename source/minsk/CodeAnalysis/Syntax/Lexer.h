#pragma once

#include <stddef.h>

#include <common/List.h>
#include <minsk/CodeAnalysis/DiagnosticBag.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>
#include <minsk/CodeAnalysis/Text/SourceText.h>
#include <sds.h>

struct Lexer
{
    struct SourceText* text;
    size_t position;
    size_t start;
    enum SyntaxKind kind;
    struct Object* value;
    struct DiagnosticBag* diagnostics;
};

struct Lexer* lexer_new(struct SourceText* source_text);
struct SyntaxToken* lexer_next_token(struct Lexer* lexer);
