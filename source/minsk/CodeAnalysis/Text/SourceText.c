#include "minsk/CodeAnalysis/Text/SourceText.h"

#include <IncludeMe.h>

static struct SourceText* source_text_new(sds text);
static struct TextLineList* parse_lines(struct SourceText* source_text, sds text);
static int get_line_break_width(sds text, int i);
static void add_line(struct TextLineList* lines, struct SourceText* source_text, int position, int line_start,
                     int line_break_width);

struct SourceText* source_text_from(sds text)
{
    return source_text_new(text);
}

int source_text_get_line_index(struct SourceText* self, int position)
{
    int lower = 0;
    int upper = self->lines->length - 1;

    while (lower <= upper)
    {
        int index = lower + (upper - lower) / 2;
        int start = self->lines->data[index]->start;

        if (position == start)
        {
            return index;
        }

        if (start > position)
        {
            upper = index - 1;
        }
        else
        {
            lower = index + 1;
        }
    }

    return lower - 1;
}

sds source_text_to_string(struct SourceText* source_text)
{
    return sdsdup(source_text->text);
}

sds source_text_to_string_bounded(struct SourceText* source_text, int start, int length)
{
    return sdsnewlen(&source_text->text[start], length);
}

sds source_text_to_string_spanned(struct SourceText* source_text, struct TextSpan* span)
{
    return source_text_to_string_bounded(source_text, span->start, span->length);
}

char source_text_at(struct SourceText* source_text, int index)
{
    return source_text->text[index];
}

int source_text_get_length(struct SourceText* source_text)
{
    return sdslen(source_text->text);
}

static struct SourceText* source_text_new(sds text)
{
    struct SourceText* source_text = mc_malloc(sizeof(struct SourceText));
    source_text->text = text;
    source_text->lines = parse_lines(source_text, text);
    return source_text;
}

static struct TextLineList* parse_lines(struct SourceText* source_text, sds text)
{
    struct TextLineList* result = mc_malloc(sizeof(struct TextLineList));
    LIST_INIT(result);
    int position = 0;
    int line_start = 0;
    while (position < sdslen(text))
    {
        int line_break_width = get_line_break_width(text, position);
        if (line_break_width == 0)
        {
            ++position;
        }
        else
        {
            add_line(result, source_text, position, line_start, line_break_width);
            position += line_break_width;
            line_start = position;
        }
    }
    if (position >= line_start)
    {
        add_line(result, source_text, position, line_start, 0);
    }

    return result;
}

static int get_line_break_width(sds text, int i)
{
    char c = text[i];
    char l = (i + 1 >= sdslen(text) ? '\0' : text[i + 1]);

    if (c == '\r' && l == '\n')
    {
        return 2;
    }
    if (c == '\r' || c == '\n')
    {
        return 1;
    }
    return 0;
}

static void add_line(struct TextLineList* lines, struct SourceText* source_text, int position, int line_start,
                     int line_break_width)
{
    int line_length = position - line_start;
    LIST_PUSH(lines, text_line_new(source_text, line_start, line_length, line_length + line_break_width));
}
