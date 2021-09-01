#include "common/Object.h"

#include <IncludeMe.h>
#include <sds.h>

const char* const OBJECT_KINDS[] = {
#define X(x) #x,
    OBJECT_KINDS_
#undef X
};

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

struct ObjectBoolean* object_new_boolean(bool value)
{
  struct ObjectBoolean* b = mc_malloc(sizeof(struct ObjectBoolean));
  b->base.kind = OBJECT_KIND_BOOLEAN;
  b->value = value;
  return b;
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
    default:
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
    case OBJECT_KIND_BOOLEAN:
      return sdsnew(OBJECT_AS_BOOLEAN(obj)->value ? "true" : "false");
    case OBJECT_KIND_NULL:
      return sdsnew("NULL");
  }
}
