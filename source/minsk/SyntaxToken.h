#pragma once

#include "Object.h"
#include "SyntaxKind.h"

struct SyntaxToken
{
  enum SyntaxKind kind;
  int position;
  char* text;
  struct Object* value;
};

struct SyntaxToken* syntax_token_new(
    enum SyntaxKind kind,
    int position,
    char* text,
    struct Object* value);
