#include "minsk/CodeAnalysis/Syntax/SyntaxToken.h"

#include <IncludeMe.h>
#include <minsk/CodeAnalysis/TextSpan.h>

struct SyntaxToken* syntax_token_new(
    enum SyntaxKind kind,
    int position,
    char* text,
    struct Object* value)
{
  struct SyntaxToken* token = mc_malloc(sizeof(struct SyntaxToken));
  token->base.kind = SYNTAX_NODE_KIND_TOKEN;
  token->kind = kind;
  token->position = position;
  token->text = text;
  token->value = value;
  return token;
}

void syntax_token_free(struct SyntaxToken* token)
{
  sdsfree(token->text);
  object_free(token->value);
  mc_free(token);
}

enum SyntaxKind syntax_token_get_kind(struct SyntaxToken* token)
{
  return token->kind;
}

struct SyntaxNodeList* syntax_token_get_children(struct SyntaxToken* token)
{
  struct SyntaxNodeList* empty = mc_malloc(sizeof(struct SyntaxNodeList));
  LIST_INIT(empty);
  return empty;
}

struct TextSpan* syntax_token_get_span(const struct SyntaxToken* token)
{
  return text_span_new(token->position, token->position + sdslen(token->text));
}
