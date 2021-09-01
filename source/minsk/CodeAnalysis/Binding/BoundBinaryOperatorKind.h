#pragma once

#define BOUND_BINARY_OPERATOR_KINDS_ \
  X(ADDITION) \
  X(SUBTRACTION) \
  X(MULTIPLICATION) \
  X(DIVISION) \
  X(LOGICAL_AND) \
  X(LOGICAL_OR) \
  X(EQUALITY) \
  X(INEQUALITY)

enum BoundBinaryOperatorKind
{
#define X(x) BOUND_BINARY_OPERATOR_KIND_##x,
  BOUND_BINARY_OPERATOR_KINDS_
#undef X
};
