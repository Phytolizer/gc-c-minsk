#include "Parser.h"

#include <stdbool.h>

#include <IncludeMe.h>
#include <common/List.h>
#include <minsk/CodeAnalysis/Syntax/AssignmentExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/NameExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxFacts.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>
#include <sds.h>

#include "Lexer.h"

static struct SyntaxToken* peek(struct Parser* parser, int offset)
    __attribute__((const));
static struct SyntaxToken* current(struct Parser* parser)
    __attribute__((const));
static struct SyntaxToken* next_token(struct Parser* parser);
static struct SyntaxToken* match_token(
    struct Parser* parser,
    enum SyntaxKind kind);

static struct ExpressionSyntax* parse_expression(struct Parser* parser);
static struct ExpressionSyntax* parse_assignment_expression(
    struct Parser* parser);
static struct ExpressionSyntax* parse_binary_expression(
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
  parser->diagnostics = diagnostic_bag_new();
  diagnostic_bag_add_range(parser->diagnostics, lexer->diagnostics);
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

  diagnostic_bag_report_unexpected_token(
      parser->diagnostics,
      syntax_token_get_span(current(parser)),
      current(parser)->kind,
      kind);
  return syntax_token_new(
      kind,
      current(parser)->position,
      sdsempty(),
      OBJECT_NULL());
}

static struct ExpressionSyntax* parse_expression(struct Parser* parser)
{
  return parse_assignment_expression(parser);
}

static struct ExpressionSyntax* parse_assignment_expression(
    struct Parser* parser)
{
  if (peek(parser, 0)->kind == SYNTAX_KIND_IDENTIFIER_TOKEN
      && peek(parser, 1)->kind == SYNTAX_KIND_EQUALS_TOKEN)
  {
    struct SyntaxToken* identifier_token = next_token(parser);
    struct SyntaxToken* equals_token = next_token(parser);
    struct ExpressionSyntax* right = parse_assignment_expression(parser);
    return (struct ExpressionSyntax*)
        assignment_expression_new(identifier_token, equals_token, right);
  }

  return parse_binary_expression(parser, 0);
}

static struct ExpressionSyntax* parse_binary_expression(
    struct Parser* parser,
    int parent_precedence)
{
  struct ExpressionSyntax* left;
  int unary_operator_prec = unary_operator_precedence(current(parser)->kind);
  if (unary_operator_prec != 0 && unary_operator_prec >= parent_precedence)
  {
    struct SyntaxToken* operator_token = next_token(parser);
    struct ExpressionSyntax* operand
        = parse_binary_expression(parser, unary_operator_prec);
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
    struct ExpressionSyntax* right
        = parse_binary_expression(parser, precedence);
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
        struct ExpressionSyntax* expression = parse_expression(parser);
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
    case SYNTAX_KIND_IDENTIFIER_TOKEN:
      {
        struct SyntaxToken* identifier_token = next_token(parser);
        return (struct ExpressionSyntax*)name_expression_syntax_new(
            identifier_token);
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
