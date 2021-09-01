#include "SyntaxFacts.h"

#include <string.h>

int unary_operator_precedence(enum SyntaxKind kind)
{
  switch (kind)
  {
    case SYNTAX_KIND_PLUS_TOKEN:
    case SYNTAX_KIND_MINUS_TOKEN:
    case SYNTAX_KIND_BANG_TOKEN:
      return 5;
    default:
      return 0;
  }
}

int binary_operator_precedence(enum SyntaxKind kind)
{
  switch (kind)
  {
    case SYNTAX_KIND_STAR_TOKEN:
    case SYNTAX_KIND_SLASH_TOKEN:
      return 4;
    case SYNTAX_KIND_PLUS_TOKEN:
    case SYNTAX_KIND_MINUS_TOKEN:
      return 3;
    case SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN:
      return 2;
    case SYNTAX_KIND_PIPE_PIPE_TOKEN:
      return 1;
    default:
      return 0;
  }
}

enum SyntaxKind keyword_kind(sds text)
{
  if (strcmp(text, "true") == 0)
  {
    return SYNTAX_KIND_TRUE_KEYWORD;
  }
  if (strcmp(text, "false") == 0)
  {
    return SYNTAX_KIND_FALSE_KEYWORD;
  }
  return SYNTAX_KIND_IDENTIFIER_TOKEN;
}
