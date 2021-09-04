#include "minsk/CodeAnalysis/TextSpan.h"

#include <IncludeMe.h>

struct TextSpan* text_span_new(int start, int length)
{
  struct TextSpan* span = mc_malloc(sizeof(struct TextSpan));
  span->start = start;
  span->length = length;
  return span;
}

int text_span_end(const struct TextSpan* span)
{
  return span->start + span->length;
}
