#include "BoundUnaryOperatorKind.h"

const char* const BOUND_UNARY_OPERATOR_KINDS[] = {
#define X(x) #x,
    BOUND_UNARY_OPERATOR_KINDS_
#undef X
};
