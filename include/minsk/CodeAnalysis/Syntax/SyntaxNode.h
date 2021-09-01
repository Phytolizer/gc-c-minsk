#pragma once

#include "common/List.h"
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

extern const char* const SYNTAX_NODE_KINDS[];

struct SyntaxNode
{
  enum SyntaxNodeKind kind;
};

DECLARE_NAMED_LIST(SyntaxNodeList, struct SyntaxNode*);

enum SyntaxKind syntax_node_get_kind(struct SyntaxNode* node);
struct SyntaxNodeList* syntax_node_get_children(struct SyntaxNode* node);
