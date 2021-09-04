#include "BoundNode.h"

const char* const BOUND_NODE_KINDS[] = {
#define X(x) #x,
    BOUND_NODE_KINDS_
#undef X
};
