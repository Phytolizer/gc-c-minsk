#pragma once

struct TextSpan
{
  int start;
  int length;
};

struct TextSpan* text_span_new(int start, int length);
int text_span_end(const struct TextSpan* span);
