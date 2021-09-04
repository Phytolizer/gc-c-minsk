#pragma once

#include <common/List.h>
#include <minsk/CodeAnalysis/DiagnosticBag.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <sds.h>

struct Parser
{
  struct SyntaxTokenList* tokens;
  int position;
  struct DiagnosticBag* diagnostics;
};

struct Parser* parser_new(sds text);
void parser_free(struct Parser* parser);
struct SyntaxTree* parser_parse(struct Parser* parser);
