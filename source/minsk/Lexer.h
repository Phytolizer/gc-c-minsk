#pragma once

#include <stddef.h>

#include "SyntaxToken.h"

struct Lexer
{
  char* text;
  size_t position;
};

struct Lexer* lexer_new(char* text);
struct SyntaxToken* lexer_next_token(struct Lexer* lexer);
