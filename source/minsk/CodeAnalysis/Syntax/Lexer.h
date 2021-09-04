#pragma once

#include <stddef.h>

#include <common/List.h>
#include <minsk/CodeAnalysis/DiagnosticBag.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>
#include <sds.h>

struct Lexer
{
  sds text;
  size_t position;
  struct DiagnosticBag* diagnostics;
};

struct Lexer* lexer_new(sds text);
void lexer_free(struct Lexer* lexer);
struct SyntaxToken* lexer_next_token(struct Lexer* lexer);
