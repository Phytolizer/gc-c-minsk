#include "minsk/CodeAnalysis/Syntax/SyntaxKind.h"

const char *const SYNTAX_KINDS[] = {
#define X(x) #x,
    SYNTAX_KINDS_
#undef X
};

const enum SyntaxKind SYNTAX_KIND_VARIANTS[] = {
#define X(x) SYNTAX_KIND_##x,
    SYNTAX_KINDS_
#undef X
};
const size_t NUM_SYNTAX_KIND_VARIANTS = sizeof(SYNTAX_KIND_VARIANTS) / sizeof(enum SyntaxKind);
