#include "minsk/CodeAnalysis/Diagnostic.h"

#include <IncludeMe.h>

struct Diagnostic* diagnostic_new(struct TextSpan* span, sds message)
{
    struct Diagnostic* diagnostic = mc_malloc(sizeof(struct Diagnostic));
    diagnostic->span = span;
    diagnostic->message = message;
    return diagnostic;
}
