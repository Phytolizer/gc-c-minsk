#include "minsk/CodeAnalysis/EvaluationResult.h"

struct EvaluationResult* evaluation_result_new(
    struct StringList* diagnostics,
    struct Object* value)
{
  struct EvaluationResult* result = mc_malloc(sizeof(struct EvaluationResult));
  result->diagnostics = diagnostics;
  result->value = value;
  return result;
}
