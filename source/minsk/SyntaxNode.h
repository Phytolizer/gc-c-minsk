#pragma once

#include "SyntaxKind.h"

#define SYNTAX_NODE_KINDS_ \
  X(TOKEN) \
  X(EXPRESSION)

enum SyntaxNodeKind
{
#define X(x) SYNTAX_NODE_KIND_##x,
  SYNTAX_NODE_KINDS_
#undef X
};

struct SyntaxNode
{
  enum SyntaxNodeKind kind;
};

enum SyntaxKind syntax_node_get_kind(struct SyntaxNode* node);
