#include "Parser.h"

#include <stdbool.h>

#include <gc.h>

#include "Lexer.h"
#include "List.h"

static struct SyntaxToken* peek(struct Parser* parser, int offset);
static struct SyntaxToken* current(struct Parser* parser);

struct Parser* parser_new(sds text)
{
  struct Parser* parser = GC_MALLOC(sizeof(struct Parser));
  parser->tokens = GC_MALLOC(sizeof(struct SyntaxTokenList));
  LIST_INIT(parser->tokens);
  parser->position = 0;
  struct Lexer* lexer = lexer_new(text);
  while (true)
  {
    struct SyntaxToken* token = lexer_next_token(lexer);
    if (token->kind != SYNTAX_KIND_WHITESPACE_TOKEN
        && token->kind != SYNTAX_KIND_BAD_TOKEN)
    {
      LIST_PUSH(parser->tokens, token);
    }
    if (token->kind == SYNTAX_KIND_END_OF_FILE_TOKEN)
    {
      break;
    }
  }
  return parser;
}

static struct SyntaxToken* peek(struct Parser* parser, int offset)
{
  int index = parser->position + offset;
  if (index >= parser->tokens->length)
  {
    return parser->tokens->data[parser->tokens->length - 1];
  }
  return parser->tokens->data[index];
}

static struct SyntaxToken* current(struct Parser* parser)
{
  return peek(parser, 0);
}
