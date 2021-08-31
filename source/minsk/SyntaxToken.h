#pragma once

#include "List.h"
#include "Object.h"
#include "SyntaxKind.h"

struct SyntaxToken
{
  enum SyntaxKind kind;
  int position;
  char* text;
  struct Object* value;
};

DECLARE_NAMED_LIST(SyntaxTokenList, struct SyntaxToken*);

struct SyntaxToken* syntax_token_new(
    enum SyntaxKind kind,
    int position,
    char* text,
    struct Object* value);

enum SyntaxKind syntax_token_get_kind(struct SyntaxToken* token);
