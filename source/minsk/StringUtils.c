#include "StringUtils.h"

#include <stdarg.h>
#include <stdio.h>

#include <gc.h>

char* alloc_printf(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  size_t len = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  char* text = GC_MALLOC(len + 1);
  va_start(args, fmt);
  vsnprintf(text, len + 1, fmt, args);
  va_end(args);

  return text;
}
