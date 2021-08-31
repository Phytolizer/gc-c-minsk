#include "Lexer.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <gc.h>

#include "StringUtils.h"
#include "SyntaxToken.h"

static char current(struct Lexer* lexer)
{
  if (lexer->position >= strlen(lexer->text))
  {
    return '\0';
  }
  return lexer->text[lexer->position];
}

static void next(struct Lexer* lexer)
{
  ++lexer->position;
}

struct Lexer* lexer_new(char* text)
{
  struct Lexer* lexer = GC_MALLOC(sizeof(struct Lexer));
  lexer->text = text;
  lexer->position = 0;
  return lexer;
}

struct SyntaxToken* lexer_next_token(struct Lexer* lexer)
{
  if (lexer->position >= strlen(lexer->text))
  {
    return syntax_token_new(
        SYNTAX_KIND_END_OF_FILE_TOKEN,
        lexer->position,
        "",
        OBJECT_NULL());
  }

  if (isdigit(current(lexer)))
  {
    int start = lexer->position;
    while (isdigit(current(lexer)))
    {
      next(lexer);
    }

    int length = lexer->position - start;
    char* text = GC_MALLOC(length + 1);
    strncpy(text, &lexer->text[start], length);
    long value = strtol(text, NULL, 10);
    if (errno == ERANGE || value < INT_MIN || value > INT_MAX)
    {
      // TODO
    }
    return syntax_token_new(
        SYNTAX_KIND_NUMBER_TOKEN,
        start,
        text,
        OBJECT_INTEGER(value));
  }

  if (isspace(current(lexer)))
  {
    int start = lexer->position;
    while (isspace(current(lexer)))
    {
      next(lexer);
    }

    int length = lexer->position - start;
    char* text = GC_MALLOC(length + 1);
    strncpy(text, &lexer->text[start], length);
    return syntax_token_new(
        SYNTAX_KIND_WHITESPACE_TOKEN,
        start,
        text,
        OBJECT_NULL());
  }

  switch (current(lexer))
  {
    case '+':
      return syntax_token_new(
          SYNTAX_KIND_PLUS_TOKEN,
          lexer->position++,
          GC_STRDUP("+"),
          OBJECT_NULL());
    case '-':
      return syntax_token_new(
          SYNTAX_KIND_MINUS_TOKEN,
          lexer->position++,
          GC_STRDUP("-"),
          OBJECT_NULL());
    case '*':
      return syntax_token_new(
          SYNTAX_KIND_STAR_TOKEN,
          lexer->position++,
          GC_STRDUP("*"),
          OBJECT_NULL());
    case '/':
      return syntax_token_new(
          SYNTAX_KIND_SLASH_TOKEN,
          lexer->position++,
          GC_STRDUP("/"),
          OBJECT_NULL());
    case '(':
      return syntax_token_new(
          SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN,
          lexer->position++,
          GC_STRDUP("("),
          OBJECT_NULL());
    case ')':
      return syntax_token_new(
          SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN,
          lexer->position++,
          GC_STRDUP(")"),
          OBJECT_NULL());
  }

  return syntax_token_new(
      SYNTAX_KIND_BAD_TOKEN,
      lexer->position++,
      alloc_printf("%c", lexer->text[lexer->position - 1]),
      OBJECT_NULL());
}
