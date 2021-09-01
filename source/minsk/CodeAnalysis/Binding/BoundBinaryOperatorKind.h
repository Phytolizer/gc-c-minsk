#pragma once

#define BOUND_BINARY_OPERATOR_KINDS_ \
  X(ADDITION) \
  X(SUBTRACTION) \
  X(MULTIPLICATION) \
  X(DIVISION)

enum BoundBinaryOperatorKind
{
#define X(x) BOUND_BINARY_OPERATOR_KIND_##x,
  BOUND_BINARY_OPERATOR_KINDS_
#undef X
};
