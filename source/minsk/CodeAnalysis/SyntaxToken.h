#pragma once

#include <sds.h>

#include "List.h"
#include "Object.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"

struct SyntaxToken
{
  struct SyntaxNode base;
  enum SyntaxKind kind;
  int position;
  sds text;
  struct Object* value;
};

DECLARE_NAMED_LIST(SyntaxTokenList, struct SyntaxToken*);

struct SyntaxToken* syntax_token_new(
    enum SyntaxKind kind,
    int position,
    sds text,
    struct Object* value);
void syntax_token_free(struct SyntaxToken* token);

enum SyntaxKind syntax_token_get_kind(struct SyntaxToken* token);
struct SyntaxNodeList* syntax_token_get_children(struct SyntaxToken* token);
