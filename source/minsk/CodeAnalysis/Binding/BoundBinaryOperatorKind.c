#include "BoundBinaryOperatorKind.h"

const char* const BOUND_BINARY_OPERATOR_KINDS[] = {
#define X(x) #x,
    BOUND_BINARY_OPERATOR_KINDS_
#undef X
};
