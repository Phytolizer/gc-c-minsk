#include "minsk/CodeAnalysis/VariableStore.h"

#include <stdio.h>
#include <string.h>

#include <IncludeMe.h>

#define INITIAL_NUM_ENTRIES 8

static struct VariableStoreEntry *find_entry_for(struct VariableStoreEntry *entries, long num_entries,
                                                 struct VariableSymbol *name);

struct VariableStore *variable_store_new(void)
{
    struct VariableStore *store = mc_malloc(sizeof(struct VariableStore));
    store->num_entries = INITIAL_NUM_ENTRIES;
    store->entries = mc_malloc(store->num_entries * sizeof(struct VariableStoreEntry));
    memset(store->entries, 0, store->num_entries * sizeof(struct VariableStoreEntry));
    store->count = 0;
    return store;
}

void variable_store_insert_or_assign(struct VariableStore *store, struct VariableSymbol *symbol, struct Object *value)
{
    if ((double)store->count / (double)store->num_entries > 0.75)
    {
        long old_num_entries = store->num_entries;
        store->num_entries *= 2;
        struct VariableStoreEntry *new_entries = mc_malloc(store->num_entries * sizeof(struct VariableStoreEntry));
        memset(new_entries, 0, store->num_entries * sizeof(struct VariableStoreEntry));
        for (long i = 0; i < old_num_entries; ++i)
        {
            if (store->entries[i].symbol)
            {
                struct VariableStoreEntry *entry =
                    find_entry_for(new_entries, store->num_entries, store->entries[i].symbol);
                entry->symbol = store->entries[i].symbol;
                entry->value = store->entries[i].value;
            }
        }
        store->entries = new_entries;
    }
    struct VariableStoreEntry *entry = find_entry_for(store->entries, store->num_entries, symbol);
    entry->symbol = symbol;
    entry->value = value;
}

struct Object **variable_store_lookup(struct VariableStore *store, struct VariableSymbol *symbol)
{
    struct VariableStoreEntry *entry = find_entry_for(store->entries, store->num_entries, symbol);
    if (entry->symbol == NULL)
    {
        return NULL;
    }
    return &entry->value;
}

void variable_store_dump(const struct VariableStore *store)
{
    for (long i = 0; i < store->num_entries; ++i)
    {
        struct VariableStoreEntry *entry = &store->entries[i];
        printf("[%2li] : ", i);
        if (entry->symbol)
        {
            printf("%s => %s", entry->symbol->name, object_to_string(entry->value));
        }
        else
        {
            printf("-- empty --");
        }
        printf("\n");
    }
}

static struct VariableStoreEntry *find_entry_for(struct VariableStoreEntry *entries, long num_entries,
                                                 struct VariableSymbol *symbol)
{
    uint64_t hash = variable_symbol_hash(symbol);
    hash %= num_entries;
    while (entries[hash].symbol != NULL && sdscmp(entries[hash].symbol->name, symbol->name) != 0)
    {
        hash = (hash + 1) % num_entries;
    }
    return &entries[hash];
}
