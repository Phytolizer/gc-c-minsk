#include "minsk/CodeAnalysis/Syntax/SyntaxKind.h"

const char* const SYNTAX_KINDS[] = {
#define X(x) #x,
    SYNTAX_KINDS_
#undef X
};
