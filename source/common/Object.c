#include "common/Object.h"

#include <IncludeMe.h>
#include <assert.h>
#include <sds.h>
#include <string.h>

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

struct ObjectString* object_new_string(sds value)
{
    struct ObjectString* s = mc_malloc(sizeof(struct ObjectString));
    s->base.kind = OBJECT_KIND_STRING;
    s->value = value;
    return s;
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

bool objects_equal(struct Object* left, struct Object* right)
{
    switch (left->kind)
    {
    case OBJECT_KIND_NULL:
        return right->kind == OBJECT_KIND_NULL;
    case OBJECT_KIND_INTEGER:
        return OBJECT_IS_INTEGER(right) && OBJECT_AS_INTEGER(left)->value == OBJECT_AS_INTEGER(right)->value;
    case OBJECT_KIND_BOOLEAN:
        return OBJECT_IS_BOOLEAN(right) && OBJECT_AS_BOOLEAN(left)->value == OBJECT_AS_BOOLEAN(right)->value;
    case OBJECT_KIND_STRING:
        return OBJECT_IS_STRING(right) && strcmp(OBJECT_AS_STRING(left)->value, OBJECT_AS_STRING(right)->value) == 0;
    }
    assert(false && "Corrupt object kind");
}

sds object_to_string(const struct Object* obj)
{
    switch (obj->kind)
    {
    case OBJECT_KIND_INTEGER:
        return sdsfromlonglong(OBJECT_AS_INTEGER(obj)->value);
    case OBJECT_KIND_BOOLEAN:
        return sdsnew(OBJECT_AS_BOOLEAN(obj)->value ? "true" : "false");
    case OBJECT_KIND_NULL:
        return sdsnew("NULL");
    case OBJECT_KIND_STRING:
        return OBJECT_AS_STRING(obj)->value;
    }
    assert(false && "Corrupt object kind");
}
