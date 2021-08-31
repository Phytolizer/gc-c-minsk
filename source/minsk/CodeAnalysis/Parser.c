#include "Parser.h"

#include <stdbool.h>

#include <common/List.h>
#include <minsk/CodeAnalysis/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/NumberExpressionSyntax.h>
#include <minsk/CodeAnalysis/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/SyntaxTree.h>

#include "IncludeMe.h"
#include "Lexer.h"
#include "sds.h"

static struct SyntaxToken* peek(struct Parser* parser, int offset);
static struct SyntaxToken* current(struct Parser* parser);
static struct SyntaxToken* next_token(struct Parser* parser);
static struct SyntaxToken* match_token(
    struct Parser* parser,
    enum SyntaxKind kind);

static struct ExpressionSyntax* parse_expression(struct Parser* parser);
static struct ExpressionSyntax* parse_term(struct Parser* parser);
static struct ExpressionSyntax* parse_factor(struct Parser* parser);
static struct ExpressionSyntax* parse_primary_expression(struct Parser* parser);

struct Parser* parser_new(sds text)
{
  struct Parser* parser = mc_malloc(sizeof(struct Parser));
  parser->tokens = mc_malloc(sizeof(struct SyntaxTokenList));
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
  parser->diagnostics = mc_malloc(sizeof(struct StringList));
  LIST_INIT(parser->diagnostics);
  for (long i = 0; i < lexer->diagnostics->length; ++i)
  {
    LIST_PUSH(parser->diagnostics, lexer->diagnostics->data[i]);
  }
  return parser;
}

void parser_free(struct Parser* parser)
{
  LIST_DEINIT(parser->tokens);
  mc_free(parser->tokens);
  mc_free(parser);
}

struct SyntaxTree* parser_parse(struct Parser* parser)
{
  struct ExpressionSyntax* expression = parse_expression(parser);
  struct SyntaxToken* end_of_file_token
      = match_token(parser, SYNTAX_KIND_END_OF_FILE_TOKEN);
  return syntax_tree_new(parser->diagnostics, expression, end_of_file_token);
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

  LIST_PUSH(
      parser->diagnostics,
      sdscatprintf(
          sdsempty(),
          "ERROR: Unexpected token <%s>, expected <%s>",
          SYNTAX_KINDS[current(parser)->kind],
          SYNTAX_KINDS[kind]));
  return syntax_token_new(
      kind,
      current(parser)->position,
      sdsempty(),
      OBJECT_NULL());
}

static struct ExpressionSyntax* parse_expression(struct Parser* parser)
{
  return parse_term(parser);
}

static struct ExpressionSyntax* parse_term(struct Parser* parser)
{
  struct ExpressionSyntax* left = parse_factor(parser);
  while (current(parser)->kind == SYNTAX_KIND_PLUS_TOKEN
         || current(parser)->kind == SYNTAX_KIND_MINUS_TOKEN)
  {
    struct SyntaxToken* operator_token = next_token(parser);
    struct ExpressionSyntax* right = parse_factor(parser);
    left = (struct ExpressionSyntax*)
        binary_expression_syntax_new(left, operator_token, right);
  }

  return left;
}

static struct ExpressionSyntax* parse_factor(struct Parser* parser)
{
  struct ExpressionSyntax* left = parse_primary_expression(parser);
  while (current(parser)->kind == SYNTAX_KIND_STAR_TOKEN
         || current(parser)->kind == SYNTAX_KIND_SLASH_TOKEN)
  {
    struct SyntaxToken* operator_token = next_token(parser);
    struct ExpressionSyntax* right = parse_primary_expression(parser);
    left = (struct ExpressionSyntax*)
        binary_expression_syntax_new(left, operator_token, right);
  }

  return left;
}

static struct ExpressionSyntax* parse_primary_expression(struct Parser* parser)
{
  if (current(parser)->kind == SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN)
  {
    struct SyntaxToken* open_parenthesis_token = next_token(parser);
    struct ExpressionSyntax* expression = parse_expression(parser);
    struct SyntaxToken* close_parenthesis_token
        = match_token(parser, SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN);
    return (struct ExpressionSyntax*)parenthesized_expression_syntax_new(
        open_parenthesis_token,
        expression,
        close_parenthesis_token);
  }
  struct SyntaxToken* number_token
      = match_token(parser, SYNTAX_KIND_NUMBER_TOKEN);
  return (struct ExpressionSyntax*)number_expression_syntax_new(number_token);
}
