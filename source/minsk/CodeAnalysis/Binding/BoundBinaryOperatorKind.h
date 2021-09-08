#pragma once

#define BOUND_BINARY_OPERATOR_KINDS_ \
    X(ADDITION) \
    X(SUBTRACTION) \
    X(MULTIPLICATION) \
    X(DIVISION) \
    X(LOGICAL_AND) \
    X(LOGICAL_OR) \
    X(EQUALITY) \
    X(INEQUALITY) \
    X(LESS) \
    X(LESS_OR_EQUALS) \
    X(GREATER) \
    X(GREATER_OR_EQUALS)

enum BoundBinaryOperatorKind
{
#define X(x) BOUND_BINARY_OPERATOR_KIND_##x,
    BOUND_BINARY_OPERATOR_KINDS_
#undef X
};
