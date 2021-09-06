#include "minsk/CodeAnalysis/Text/TextLine.h"

#include <IncludeMe.h>
#include <minsk/CodeAnalysis/Text/SourceText.h>

struct TextLine* text_line_new(
    struct SourceText* text,
    int start,
    int length,
    int length_including_line_break)
{
  struct TextLine* line = mc_malloc(sizeof(struct TextLine));
  line->text = text;
  line->start = start;
  line->length = length;
  line->length_including_line_break = length_including_line_break;
  return line;
}

int text_line_get_end(struct TextLine* line)
{
  return line->start + line->length;
}

struct TextSpan* text_line_get_span(struct TextLine* line)
{
  return text_span_new(line->start, line->length);
}

struct TextSpan* text_line_get_span_including_line_break(struct TextLine* line)
{
  return text_span_new(line->start, line->length_including_line_break);
}

sds text_line_to_string(struct TextLine* line)
{
  return source_text_to_string_spanned(line->text, text_line_get_span(line));
}
