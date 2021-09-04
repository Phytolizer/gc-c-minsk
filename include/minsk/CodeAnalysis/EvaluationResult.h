#pragma once

#include <common/List.h>
#include <common/Object.h>
#include <minsk/CodeAnalysis/Diagnostic.h>

struct EvaluationResult
{
  struct DiagnosticList* diagnostics;
  struct Object* value;
};

struct EvaluationResult* evaluation_result_new(
    struct DiagnosticList* diagnostics,
    struct Object* value);
