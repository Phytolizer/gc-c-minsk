#include "common/VariableStore.h"

#include <IncludeMe.h>

#define MAX_LOAD 0.75

#define FNV_OFFSET_BASIS 0xCBF29CE484222325
#define FNV_PRIME 0x100000001B3

static uint64_t hash(sds name)
{
  uint64_t hash = FNV_OFFSET_BASIS;
  for (char* c = name; *c; ++c)
  {
    hash ^= *c;
    hash *= FNV_PRIME;
  }
  return hash;
}

static struct VariableStoreEntry* find_empty_entry(
    struct VariableStoreEntry* entries,
    long num_entries,
    sds name);

static struct VariableStoreEntry* lookup_entry(
    struct VariableStore* store,
    sds name);

struct VariableStore* variable_store_new(void)
{
  struct VariableStore* store = mc_malloc(sizeof(struct VariableStore));
  store->entries = NULL;
  store->count = 0;
  store->num_entries = 0;
  return store;
}

void variable_store_insert(
    struct VariableStore* store,
    sds name,
    struct Object* value)
{
  if (store->num_entries == 0
      || (double)store->count / store->num_entries > MAX_LOAD)
  {
    long old_num_entries = store->num_entries;
    if (store->num_entries == 0)
    {
      store->num_entries = 8;
    }
    store->num_entries *= 2;
    struct VariableStoreEntry* new_entries
        = mc_malloc(store->num_entries * sizeof(struct VariableStoreEntry));
    for (long i = 0; i < old_num_entries; ++i)
    {
      struct VariableStoreEntry* entry = find_empty_entry(
          new_entries,
          store->num_entries,
          store->entries[i].name);
      entry->name = store->entries[i].name;
      entry->value = store->entries[i].value;
    }
    store->entries = new_entries;
  }
  struct VariableStoreEntry* entry
      = find_empty_entry(store->entries, store->num_entries, name);
  entry->name = name;
  entry->value = value;
}

struct Object** variable_store_lookup(struct VariableStore* store, sds name)
{
  struct VariableStoreEntry* entry = lookup_entry(store, name);
  if (!entry || entry->name == NULL)
  {
    return NULL;
  }
  return &entry->value;
}

static struct VariableStoreEntry* find_empty_entry(
    struct VariableStoreEntry* entries,
    long num_entries,
    sds name)
{
  if (num_entries == 0)
  {
    return NULL;
  }
  uint64_t h = hash(name) % num_entries;
  while (entries[h].name != NULL)
  {
    h = (h + 1) % num_entries;
  }
  return &entries[h];
}

static struct VariableStoreEntry* lookup_entry(
    struct VariableStore* store,
    sds name)
{
  if (store->num_entries == 0)
  {
    return NULL;
  }
  uint64_t h = hash(name) % store->num_entries;
  while (store->entries[h].name != NULL
         && sdscmp(store->entries[h].name, name) != 0)
  {
    h = (h + 1) % store->num_entries;
  }
  return &store->entries[h];
}
