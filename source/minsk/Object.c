#include "Object.h"

#include <gc.h>

#include "StringUtils.h"

struct Object* object_new_null(void)
{
  struct Object* o = GC_MALLOC(sizeof(struct Object));
  o->kind = OBJECT_KIND_NULL;
  return o;
}

struct ObjectInteger* object_new_integer(int value)
{
  struct ObjectInteger* i = GC_MALLOC(sizeof(struct ObjectInteger));
  i->base.kind = OBJECT_KIND_INTEGER;
  i->value = value;
  return i;
}

char* object_to_string(const struct Object* obj)
{
  switch (obj->kind)
  {
    case OBJECT_KIND_INTEGER:
      return alloc_printf("%d", OBJECT_AS_INTEGER(obj)->value);
    case OBJECT_KIND_NULL:
      return alloc_printf("NULL");
  }
}
