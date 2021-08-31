#pragma once

#include <sds.h>

#include "ExpressionSyntax.h"
struct Parser
{
  struct SyntaxTokenList* tokens;
  int position;
};

struct Parser* parser_new(sds text);
void parser_free(struct Parser* parser);
struct ExpressionSyntax* parser_parse(struct Parser* parser);
