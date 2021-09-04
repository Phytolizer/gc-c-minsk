#pragma once

#include <common/List.h>
#include <sds.h>

#include "TextSpan.h"

struct Diagnostic
{
  struct TextSpan* span;
  sds message;
};

DECLARE_NAMED_LIST(DiagnosticList, struct Diagnostic*);

struct Diagnostic* diagnostic_new(struct TextSpan* span, sds message);
