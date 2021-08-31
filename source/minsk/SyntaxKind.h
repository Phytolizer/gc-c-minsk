#pragma once

#define SYNTAX_KINDS_ \
  X(NUMBER_TOKEN) \
  X(WHITESPACE_TOKEN) \
  X(PLUS_TOKEN) \
  X(MINUS_TOKEN) \
  X(STAR_TOKEN) \
  X(SLASH_TOKEN) \
  X(OPEN_PARENTHESIS_TOKEN) \
  X(CLOSE_PARENTHESIS_TOKEN) \
  X(BAD_TOKEN) \
  X(END_OF_FILE_TOKEN)

extern const char* const SYNTAX_KINDS[];

enum SyntaxKind
{
#define X(x) SYNTAX_KIND_##x,
  SYNTAX_KINDS_
#undef X
};