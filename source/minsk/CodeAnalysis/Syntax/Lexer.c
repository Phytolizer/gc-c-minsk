#include "Lexer.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <IncludeMe.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxFacts.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>

#include "minsk/CodeAnalysis/Syntax/SyntaxKind.h"

__attribute__((const)) static char peek(struct Lexer* lexer, int offset)
{
  int index = lexer->position + offset;
  if (index >= sdslen(lexer->text))
  {
    return '\0';
  }
  return lexer->text[index];
}

__attribute__((const)) static char current(struct Lexer* lexer)
{
  return peek(lexer, 0);
}

__attribute__((const)) static char lookahead(struct Lexer* lexer)
{
  return peek(lexer, 1);
}

static void next(struct Lexer* lexer)
{
  ++lexer->position;
}

static void read_number(struct Lexer* lexer)
{
  while (isdigit(current(lexer)))
  {
    next(lexer);
  }

  int length = lexer->position - lexer->start;
  sds text = sdsnewlen(&lexer->text[lexer->start], length);
  long value = strtol(text, NULL, 10);
  if (errno == ERANGE || value < INT_MIN || value > INT_MAX)
  {
    diagnostic_bag_report_invalid_number(
        lexer->diagnostics,
        text_span_new(lexer->start, length),
        text,
        OBJECT_KIND_INTEGER);
  }
  lexer->value = OBJECT_INTEGER(value);
  lexer->kind = SYNTAX_KIND_NUMBER_TOKEN;
}

static void read_whitespace(struct Lexer* lexer)
{
  while (isspace(current(lexer)))
  {
    next(lexer);
  }
  lexer->kind = SYNTAX_KIND_WHITESPACE_TOKEN;
}

static void read_identifier_or_keyword(struct Lexer* lexer)
{
  while (isalpha(current(lexer)))
  {
    next(lexer);
  }

  int length = lexer->position - lexer->start;
  sds text = sdsnewlen(&lexer->text[lexer->start], length);
  lexer->kind = keyword_kind(text);
}

struct Lexer* lexer_new(sds text)
{
  struct Lexer* lexer = mc_malloc(sizeof(struct Lexer));
  lexer->text = text;
  lexer->position = 0;
  lexer->start = 0;
  lexer->kind = SYNTAX_KIND_BAD_TOKEN;
  lexer->value = OBJECT_NULL();
  lexer->diagnostics = diagnostic_bag_new();
  return lexer;
}

void lexer_free(struct Lexer* lexer)
{
  sdsfree(lexer->text);
  mc_free(lexer);
}

struct SyntaxToken* lexer_next_token(struct Lexer* lexer)
{
  lexer->start = lexer->position;
  lexer->kind = SYNTAX_KIND_BAD_TOKEN;
  lexer->value = OBJECT_NULL();

  switch (current(lexer))
  {
    case '+':
      lexer->position++;
      lexer->kind = SYNTAX_KIND_PLUS_TOKEN;
      break;
    case '-':
      lexer->position++;
      lexer->kind = SYNTAX_KIND_MINUS_TOKEN;
      break;
    case '*':
      lexer->position++;
      lexer->kind = SYNTAX_KIND_STAR_TOKEN;
      break;
    case '/':
      lexer->position++;
      lexer->kind = SYNTAX_KIND_SLASH_TOKEN;
      break;
    case '(':
      lexer->position++;
      lexer->kind = SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN;
      break;
    case ')':
      lexer->position++;
      lexer->kind = SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN;
      break;
    case '!':
      if (lookahead(lexer) == '=')
      {
        lexer->position += 2;
        lexer->kind = SYNTAX_KIND_BANG_EQUALS_TOKEN;
      }
      else
      {
        lexer->position++;
        lexer->kind = SYNTAX_KIND_BANG_TOKEN;
      }
      break;
    case '&':
      if (lookahead(lexer) == '&')
      {
        lexer->position += 2;
        lexer->kind = SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN;
      }
      break;
    case '|':
      if (lookahead(lexer) == '|')
      {
        lexer->position += 2;
        lexer->kind = SYNTAX_KIND_PIPE_PIPE_TOKEN;
      }
      break;
    case '=':
      if (lookahead(lexer) == '=')
      {
        lexer->position += 2;
        lexer->kind = SYNTAX_KIND_EQUALS_EQUALS_TOKEN;
      }
      else
      {
        lexer->position++;
        lexer->kind = SYNTAX_KIND_EQUALS_TOKEN;
      }
      break;
    case '\0':
      lexer->kind = SYNTAX_KIND_END_OF_FILE_TOKEN;
      break;
    case '0' ... '9':
      read_number(lexer);
      break;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      read_whitespace(lexer);
      break;
    case 'a' ... 'z':
    case 'A' ... 'Z':
      read_identifier_or_keyword(lexer);
      break;
    default:
      diagnostic_bag_report_bad_character(
          lexer->diagnostics,
          lexer->start,
          current(lexer));
      lexer->position++;
  }

  int length = lexer->position - lexer->start;
  sds text = syntax_facts_get_text(lexer->kind);
  if (!text)
  {
    text = sdsnewlen(&lexer->text[lexer->start], length);
  }
  return syntax_token_new(lexer->kind, lexer->start, text, lexer->value);
}
