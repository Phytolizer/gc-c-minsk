#pragma once

#define BOUND_UNARY_OPERATOR_KINDS_                                                                                    \
    X(IDENTITY)                                                                                                        \
    X(NEGATION)                                                                                                        \
    X(LOGICAL_NEGATION)

enum BoundUnaryOperatorKind
{
#define X(x) BOUND_UNARY_OPERATOR_KIND_##x,
    BOUND_UNARY_OPERATOR_KINDS_
#undef X
};
