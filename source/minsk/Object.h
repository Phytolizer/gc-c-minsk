#pragma once

#define OBJECT_KINDS_ \
  X(INTEGER) \
  X(NULL)

enum ObjectKind
{
#define X(x) OBJECT_KIND_##x,
  OBJECT_KINDS_
#undef X
};

struct Object
{
  enum ObjectKind kind;
};

struct ObjectInteger
{
  struct Object base;
  int value;
};

struct Object* object_new_null(void);
struct ObjectInteger* object_new_integer(int value);

char* object_to_string(const struct Object* obj);

#define OBJECT_INTEGER(i) ((struct Object*)object_new_integer(i))
#define OBJECT_NULL() ((struct Object*)object_new_null())

#define OBJECT_IS_INTEGER(o) (((struct Object*)o)->kind == OBJECT_KIND_INTEGER)
#define OBJECT_IS_NULL(o) (((struct Object*)o)->kind == OBJECT_KIND_NULL)

#define OBJECT_AS_INTEGER(o) ((struct ObjectInteger*)o)
