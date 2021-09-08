#pragma once

#define BOUND_NODE_TYPES_ \
  X(EXPRESSION) \
  X(STATEMENT)

enum BoundNodeType
{
#define X(x) BOUND_NODE_TYPE_##x,
  BOUND_NODE_TYPES_
#undef X
};

#define BOUND_NODE_KINDS_ \
  X(BINARY_EXPRESSION) \
  X(LITERAL_EXPRESSION) \
  X(UNARY_EXPRESSION) \
  X(VARIABLE_EXPRESSION) \
  X(ASSIGNMENT_EXPRESSION) \
  X(BLOCK_STATEMENT) \
  X(EXPRESSION_STATEMENT) \
  X(IF_STATEMENT) \
  X(VARIABLE_DECLARATION)

enum BoundNodeKind
{
#define X(x) BOUND_NODE_KIND_##x,
  BOUND_NODE_KINDS_
#undef X
};

extern const char* const BOUND_NODE_KINDS[];

struct BoundNode
{
  enum BoundNodeType type;
};

enum BoundNodeKind bound_node_get_kind(struct BoundNode* node);
