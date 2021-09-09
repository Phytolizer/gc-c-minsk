#pragma once

#define BOUND_UNARY_OPERATOR_KINDS_ \
    X(IDENTITY) \
    X(NEGATION) \
    X(LOGICAL_NEGATION) \
    X(ONES_COMPLEMENT)

enum BoundUnaryOperatorKind
{
#define X(x) BOUND_UNARY_OPERATOR_KIND_##x,
    BOUND_UNARY_OPERATOR_KINDS_
#undef X
};

extern const char* const BOUND_UNARY_OPERATOR_KINDS[];
