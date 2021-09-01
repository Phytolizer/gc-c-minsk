#pragma once

#include <stddef.h>

#include <sds.h>

#include <common/List.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>

struct Lexer
{
  sds text;
  size_t position;
  struct StringList* diagnostics;
};

struct Lexer* lexer_new(sds text);
void lexer_free(struct Lexer* lexer);
struct SyntaxToken* lexer_next_token(struct Lexer* lexer);
