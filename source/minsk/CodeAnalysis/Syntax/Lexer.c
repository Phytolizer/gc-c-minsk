#include "Lexer.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <IncludeMe.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>

#include "SyntaxFacts.h"

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

struct Lexer* lexer_new(sds text)
{
  struct Lexer* lexer = mc_malloc(sizeof(struct Lexer));
  lexer->text = text;
  lexer->position = 0;
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
  if (lexer->position >= sdslen(lexer->text))
  {
    return syntax_token_new(
        SYNTAX_KIND_END_OF_FILE_TOKEN,
        lexer->position,
        "",
        OBJECT_NULL());
  }

  int start = lexer->position;

  if (isdigit(current(lexer)))
  {
    while (isdigit(current(lexer)))
    {
      next(lexer);
    }

    int length = lexer->position - start;
    sds text = sdsnewlen(&lexer->text[start], length);
    long value = strtol(text, NULL, 10);
    if (errno == ERANGE || value < INT_MIN || value > INT_MAX)
    {
      diagnostic_bag_report_invalid_number(
          lexer->diagnostics,
          text_span_new(start, length),
          text,
          OBJECT_KIND_INTEGER);
    }
    return syntax_token_new(
        SYNTAX_KIND_NUMBER_TOKEN,
        start,
        text,
        OBJECT_INTEGER(value));
  }

  if (isspace(current(lexer)))
  {
    while (isspace(current(lexer)))
    {
      next(lexer);
    }

    int length = lexer->position - start;
    sds text = sdsnewlen(&lexer->text[start], length);
    return syntax_token_new(
        SYNTAX_KIND_WHITESPACE_TOKEN,
        start,
        text,
        OBJECT_NULL());
  }

  if (isalpha(current(lexer)))
  {
    while (isalpha(current(lexer)))
    {
      next(lexer);
    }

    int length = lexer->position - start;
    sds text = sdsnewlen(&lexer->text[start], length);
    enum SyntaxKind kind = keyword_kind(text);
    return syntax_token_new(kind, start, text, OBJECT_NULL());
  }

  switch (current(lexer))
  {
    case '+':
      lexer->position++;
      return syntax_token_new(
          SYNTAX_KIND_PLUS_TOKEN,
          start,
          sdsnew("+"),
          OBJECT_NULL());
    case '-':
      lexer->position++;
      return syntax_token_new(
          SYNTAX_KIND_MINUS_TOKEN,
          start,
          sdsnew("-"),
          OBJECT_NULL());
    case '*':
      lexer->position++;
      return syntax_token_new(
          SYNTAX_KIND_STAR_TOKEN,
          start,
          sdsnew("*"),
          OBJECT_NULL());
    case '/':
      lexer->position++;
      return syntax_token_new(
          SYNTAX_KIND_SLASH_TOKEN,
          start,
          sdsnew("/"),
          OBJECT_NULL());
    case '(':
      lexer->position++;
      return syntax_token_new(
          SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN,
          start,
          sdsnew("("),
          OBJECT_NULL());
    case ')':
      lexer->position++;
      return syntax_token_new(
          SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN,
          start,
          sdsnew(")"),
          OBJECT_NULL());
    case '!':
      if (lookahead(lexer) == '=')
      {
        lexer->position += 2;
        return syntax_token_new(
            SYNTAX_KIND_BANG_EQUALS_TOKEN,
            start,
            sdsnew("!="),
            OBJECT_NULL());
      }
      else
      {
        lexer->position++;
        return syntax_token_new(
            SYNTAX_KIND_BANG_TOKEN,
            start,
            sdsnew("!"),
            OBJECT_NULL());
      }
    case '&':
      if (lookahead(lexer) == '&')
      {
        lexer->position += 2;
        return syntax_token_new(
            SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN,
            start,
            sdsnew("&&"),
            OBJECT_NULL());
      }
    case '|':
      if (lookahead(lexer) == '|')
      {
        lexer->position += 2;
        return syntax_token_new(
            SYNTAX_KIND_PIPE_PIPE_TOKEN,
            start,
            sdsnew("||"),
            OBJECT_NULL());
      }
    case '=':
      if (lookahead(lexer) == '=')
      {
        lexer->position += 2;
        return syntax_token_new(
            SYNTAX_KIND_EQUALS_EQUALS_TOKEN,
            start,
            sdsnew("=="),
            OBJECT_NULL());
      }
  }

  diagnostic_bag_report_bad_character(
      lexer->diagnostics,
      start,
      current(lexer));
  lexer->position++;
  return syntax_token_new(
      SYNTAX_KIND_BAD_TOKEN,
      start,
      sdscatlen(sdsempty(), &lexer->text[start], 1),
      OBJECT_NULL());
}
