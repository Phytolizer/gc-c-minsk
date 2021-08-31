#include "Parser.h"

#include <stdbool.h>

#include <gc.h>

#include "BinaryExpressionSyntax.h"
#include "Lexer.h"
#include "List.h"
#include "NumberExpressionSyntax.h"

static struct SyntaxToken* peek(struct Parser* parser, int offset);
static struct SyntaxToken* current(struct Parser* parser);
static struct SyntaxToken* next_token(struct Parser* parser);
static struct SyntaxToken* match_token(
    struct Parser* parser,
    enum SyntaxKind kind);

static struct ExpressionSyntax* parse_primary_expression(struct Parser* parser);

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

struct ExpressionSyntax* parser_parse(struct Parser* parser)
{
  struct ExpressionSyntax* left = parse_primary_expression(parser);
  while (current(parser)->kind == SYNTAX_KIND_PLUS_TOKEN
         || current(parser)->kind == SYNTAX_KIND_MINUS_TOKEN)
  {
    struct SyntaxToken* operator_token = next_token(parser);
    struct ExpressionSyntax* right = parse_primary_expression(parser);
    left = (struct ExpressionSyntax*)
        binary_expression_syntax_new(left, operator_token, right);
  }

  return left;
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

static struct SyntaxToken* next_token(struct Parser* parser)
{
  struct SyntaxToken* curr = current(parser);
  ++parser->position;
  return curr;
}

static struct SyntaxToken* match_token(
    struct Parser* parser,
    enum SyntaxKind kind)
{
  if (current(parser)->kind == kind)
  {
    return next_token(parser);
  }

  return syntax_token_new(kind, current(parser)->position, NULL, NULL);
}

static struct ExpressionSyntax* parse_primary_expression(struct Parser* parser)
{
  struct SyntaxToken* number_token
      = match_token(parser, SYNTAX_KIND_NUMBER_TOKEN);
  return (struct ExpressionSyntax*)number_expression_syntax_new(number_token);
}
