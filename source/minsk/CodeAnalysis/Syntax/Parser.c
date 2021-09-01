#include "Parser.h"

#include <stdbool.h>

#include <IncludeMe.h>
#include <common/List.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>
#include <sds.h>

#include "Lexer.h"
#include "SyntaxFacts.h"

static struct SyntaxToken* peek(struct Parser* parser, int offset)
    __attribute__((const));
static struct SyntaxToken* current(struct Parser* parser)
    __attribute__((const));
static struct SyntaxToken* next_token(struct Parser* parser);
static struct SyntaxToken* match_token(
    struct Parser* parser,
    enum SyntaxKind kind);

static struct ExpressionSyntax* parse_expression(
    struct Parser* parser,
    int parent_precedence);
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
  struct ExpressionSyntax* expression = parse_expression(parser, 0);
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

static struct ExpressionSyntax* parse_expression(
    struct Parser* parser,
    int parent_precedence)
{
  struct ExpressionSyntax* left;
  int unary_operator_prec = unary_operator_precedence(current(parser)->kind);
  if (unary_operator_prec != 0 && unary_operator_prec >= parent_precedence)
  {
    struct SyntaxToken* operator_token = next_token(parser);
    struct ExpressionSyntax* operand
        = parse_expression(parser, unary_operator_prec);
    left = (struct ExpressionSyntax*)unary_expression_syntax_new(
        operator_token,
        operand);
  }
  else
  {
    left = parse_primary_expression(parser);
  }

  while (true)
  {
    int precedence = binary_operator_precedence(current(parser)->kind);
    if (precedence == 0 || precedence <= parent_precedence)
    {
      break;
    }

    struct SyntaxToken* operator_token = next_token(parser);
    struct ExpressionSyntax* right = parse_expression(parser, precedence);
    left = (struct ExpressionSyntax*)
        binary_expression_syntax_new(left, operator_token, right);
  }

  return left;
}

static struct ExpressionSyntax* parse_primary_expression(struct Parser* parser)
{
  switch (current(parser)->kind)
  {
    case SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN:
      {
        struct SyntaxToken* open_parenthesis_token = next_token(parser);
        struct ExpressionSyntax* expression = parse_expression(parser, 0);
        struct SyntaxToken* close_parenthesis_token
            = match_token(parser, SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN);
        return (struct ExpressionSyntax*)parenthesized_expression_syntax_new(
            open_parenthesis_token,
            expression,
            close_parenthesis_token);
      }
    case SYNTAX_KIND_TRUE_KEYWORD:
    case SYNTAX_KIND_FALSE_KEYWORD:
      {
        struct SyntaxToken* keyword_token = next_token(parser);
        bool value = keyword_token->kind == SYNTAX_KIND_TRUE_KEYWORD;
        return (struct ExpressionSyntax*)literal_expression_syntax_new(
            keyword_token,
            OBJECT_BOOLEAN(value));
      }
    default:
      {
        struct SyntaxToken* number_token
            = match_token(parser, SYNTAX_KIND_NUMBER_TOKEN);
        return (struct ExpressionSyntax*)literal_expression_syntax_new(
            number_token,
            number_token->value);
      }
  }
}
