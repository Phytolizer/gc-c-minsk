#pragma once

#include <sds.h>

#include "SyntaxTree.h"
#include "List.h"

struct Parser
{
  struct SyntaxTokenList* tokens;
  int position;
  struct StringList* diagnostics;
};

struct Parser* parser_new(sds text);
void parser_free(struct Parser* parser);
struct SyntaxTree* parser_parse(struct Parser* parser);
