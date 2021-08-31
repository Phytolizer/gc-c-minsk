#include "SyntaxToken.h"

#include <gc.h>

struct SyntaxToken* syntax_token_new(
    enum SyntaxKind kind,
    int position,
    char* text,
    struct Object* value)
{
  struct SyntaxToken* token = GC_MALLOC(sizeof(struct SyntaxToken));
  token->kind = kind;
  token->position = position;
  token->text = text;
  token->value = value;
  return token;
}
