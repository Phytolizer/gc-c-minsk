#include "Object.h"

#include <sds.h>

#include "IncludeMe.h"

struct Object* object_new_null(void)
{
  struct Object* o = mc_malloc(sizeof(struct Object));
  o->kind = OBJECT_KIND_NULL;
  return o;
}

struct ObjectInteger* object_new_integer(int value)
{
  struct ObjectInteger* i = mc_malloc(sizeof(struct ObjectInteger));
  i->base.kind = OBJECT_KIND_INTEGER;
  i->value = value;
  return i;
}

void object_free(struct Object* obj)
{
  switch (obj->kind)
  {
    case OBJECT_KIND_INTEGER:
      object_integer_free((struct ObjectInteger*)obj);
      break;
    case OBJECT_KIND_NULL:
      mc_free(obj);
      break;
  }
}

void object_integer_free(struct ObjectInteger* obj)
{
  mc_free(obj);
}

char* object_to_string(const struct Object* obj)
{
  switch (obj->kind)
  {
    case OBJECT_KIND_INTEGER:
      return sdsfromlonglong(OBJECT_AS_INTEGER(obj)->value);
    case OBJECT_KIND_NULL:
      return sdsnew("NULL");
  }
}
