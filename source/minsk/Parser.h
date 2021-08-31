#pragma once

#include <sds.h>
struct Parser
{
  struct SyntaxTokenList* tokens;
  int position;
};

struct Parser* parser_new(sds text);
