#pragma once

#include <common/List.h>
#include <common/Object.h>

struct EvaluationResult
{
  struct StringList* diagnostics;
  struct Object* value;
};

struct EvaluationResult* evaluation_result_new(
    struct StringList* diagnostics,
    struct Object* value);
