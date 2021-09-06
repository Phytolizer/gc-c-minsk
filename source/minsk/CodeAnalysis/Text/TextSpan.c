#include "minsk/CodeAnalysis/Text/TextSpan.h"

#include <IncludeMe.h>

struct TextSpan* text_span_new(int start, int length)
{
  struct TextSpan* span = mc_malloc(sizeof(struct TextSpan));
  span->start = start;
  span->length = length;
  return span;
}

struct TextSpan* text_span_from_bounds(int start, int end)
{
  int length = end - start;
  return text_span_new(start, length);
}

int text_span_end(const struct TextSpan* span)
{
  return span->start + span->length;
}
