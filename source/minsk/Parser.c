#include "Parser.h"

#include <gc.h>

struct Parser* parser_new(sds text)
{
  struct Parser* parser = GC_MALLOC(sizeof(struct Parser));
  return parser;
}
