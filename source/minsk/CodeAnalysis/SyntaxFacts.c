#include "SyntaxFacts.h"

int binary_operator_precedence(enum SyntaxKind kind)
{
  switch (kind)
  {
    case SYNTAX_KIND_STAR_TOKEN:
    case SYNTAX_KIND_SLASH_TOKEN:
      return 2;
    case SYNTAX_KIND_PLUS_TOKEN:
    case SYNTAX_KIND_MINUS_TOKEN:
      return 1;
    default:
      return 0;
  }
}
