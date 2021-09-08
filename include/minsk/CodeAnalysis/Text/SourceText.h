#pragma once

#include <common/List.h>
#include <sds.h>

#include "TextLine.h"

DECLARE_NAMED_LIST(TextLineList, struct TextLine*);

struct SourceText
{
    sds text;
    struct TextLineList* lines;
};

struct SourceText* source_text_from(sds text);
int source_text_get_line_index(struct SourceText* source_text, int position);
sds source_text_to_string(struct SourceText* source_text);
sds source_text_to_string_bounded(struct SourceText* source_text, int start, int length);
sds source_text_to_string_spanned(struct SourceText* source_text, struct TextSpan* span);
__attribute__((const)) char source_text_at(struct SourceText* source_text, int index);
__attribute__((const)) int source_text_get_length(struct SourceText* source_text);
