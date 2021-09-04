#pragma once

#include <sds.h>

#include "Object.h"

struct VariableStoreEntry
{
  sds name;
  struct Object* value;
};

struct VariableStore
{
  struct VariableStoreEntry* entries;
  long count;
  long num_entries;
};

struct VariableStore* variable_store_new(void);
void variable_store_insert(
    struct VariableStore* store,
    sds name,
    struct Object* value);
struct Object** variable_store_lookup(struct VariableStore* store, sds name);
