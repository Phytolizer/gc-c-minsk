#pragma once

#include <sds.h>

#include "TextSpan.h"

struct SourceText;

struct TextLine
{
    struct SourceText *text;
    int start;
    int length;
    int length_including_line_break;
};

struct TextLine *text_line_new(struct SourceText *text, int start, int length, int length_including_line_break);
int text_line_get_end(struct TextLine *line);
struct TextSpan *text_line_get_span(struct TextLine *line);
struct TextSpan *text_line_get_span_including_line_break(struct TextLine *line);
sds text_line_to_string(struct TextLine *line);
