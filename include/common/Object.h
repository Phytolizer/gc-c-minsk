#pragma once

#include <stdbool.h>
#define OBJECT_KINDS_                                                                                                  \
    X(INTEGER)                                                                                                         \
    X(BOOLEAN)                                                                                                         \
    X(NULL)

enum ObjectKind
{
#define X(x) OBJECT_KIND_##x,
    OBJECT_KINDS_
#undef X
};

extern const char* const OBJECT_KINDS[];

struct Object
{
    enum ObjectKind kind;
};

struct ObjectInteger
{
    struct Object base;
    int value;
};

struct ObjectBoolean
{
    struct Object base;
    bool value;
};

struct Object* object_new_null(void);
struct ObjectInteger* object_new_integer(int value);
struct ObjectBoolean* object_new_boolean(bool value);
void object_free(struct Object* obj);
void object_integer_free(struct ObjectInteger* obj);

bool objects_equal(struct Object* left, struct Object* right);
char* object_to_string(const struct Object* obj);

#define OBJECT_INTEGER(i) ((struct Object*)object_new_integer(i))
#define OBJECT_BOOLEAN(b) ((struct Object*)object_new_boolean(b))
#define OBJECT_NULL() ((struct Object*)object_new_null())

#define OBJECT_IS_INTEGER(o) (((struct Object*)o)->kind == OBJECT_KIND_INTEGER)
#define OBJECT_IS_BOOLEAN(o) (((struct Object*)o)->kind == OBJECT_KIND_BOOLEAN)
#define OBJECT_IS_NULL(o) (((struct Object*)o)->kind == OBJECT_KIND_NULL)

#define OBJECT_AS_INTEGER(o) ((struct ObjectInteger*)o)
#define OBJECT_AS_BOOLEAN(o) ((struct ObjectBoolean*)o)
