#pragma once

#include <common/Object.h>
#include <minsk/CodeAnalysis/VariableSymbol.h>
#include <sds.h>

struct VariableStoreEntry
{
    struct VariableSymbol *symbol;
    struct Object *value;
};

struct VariableStore
{
    struct VariableStoreEntry *entries;
    long count;
    long num_entries;
};

struct VariableStore *variable_store_new(void);
void variable_store_insert_or_assign(struct VariableStore *store, struct VariableSymbol *symbol, struct Object *value);
struct Object **variable_store_lookup(struct VariableStore *store, struct VariableSymbol *symbol);
void variable_store_dump(const struct VariableStore *store);
