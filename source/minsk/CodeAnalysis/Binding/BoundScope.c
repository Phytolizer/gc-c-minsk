#include "BoundScope.h"

#include <IncludeMe.h>
#include <common/Hashing.h>

#define INITIAL_SCOPE_SIZE 8

static struct BoundScopeVariables* bound_scope_variables_new(void);
static struct BoundScopeVariableEntry* bound_scope_variables_find_entry(struct BoundScopeVariableEntry* entries,
                                                                        long num_entries, sds name);

struct BoundScope* bound_scope_new(struct BoundScope* parent)
{
    struct BoundScope* scope = mc_malloc(sizeof(struct BoundScope));
    scope->variables = bound_scope_variables_new();
    scope->parent = parent;
    return scope;
}

struct VariableSymbol** bound_scope_try_lookup(struct BoundScope* scope, sds name)
{
    struct BoundScopeVariableEntry* entry =
        bound_scope_variables_find_entry(scope->variables->entries, scope->variables->num_entries, name);
    if (!entry || entry->name == NULL)
    {
        if (scope->parent)
        {
            return bound_scope_try_lookup(scope->parent, name);
        }
        else
        {
            return false;
        }
    }
    return &entry->value;
}

bool bound_scope_try_declare(struct BoundScope* scope, struct VariableSymbol* variable)
{
    struct BoundScopeVariableEntry* entry =
        bound_scope_variables_find_entry(scope->variables->entries, scope->variables->num_entries, variable->name);
    if (entry != NULL && entry->name != NULL)
    {
        return false;
    }
    if (scope->variables->num_entries == 0 ||
        (double)scope->variables->count / (double)scope->variables->num_entries > 0.75)
    {
        long old_num_entries = scope->variables->num_entries;
        scope->variables->num_entries = (scope->variables->num_entries == 0) ? 8 : scope->variables->num_entries * 2;
        struct BoundScopeVariableEntry* new_entries =
            mc_malloc(scope->variables->num_entries * sizeof(struct BoundScopeVariableEntry));
        for (long i = 0; i < old_num_entries; ++i)
        {
            struct BoundScopeVariableEntry* entry = bound_scope_variables_find_entry(
                new_entries, scope->variables->num_entries, scope->variables->entries[i].name);
            entry->name = scope->variables->entries[i].name;
            entry->value = scope->variables->entries[i].value;
        }
        scope->variables->entries = new_entries;
        entry =
            bound_scope_variables_find_entry(scope->variables->entries, scope->variables->num_entries, variable->name);
    }
    entry->name = variable->name;
    entry->value = variable;
    return true;
}

struct VariableSymbolList* bound_scope_get_declared_variables(struct BoundScope* scope)
{
    struct VariableSymbolList* variables = mc_malloc(sizeof(struct VariableSymbolList));
    LIST_INIT(variables);
    for (long i = 0; i < scope->variables->num_entries; ++i)
    {
        if (scope->variables->entries[i].name)
        {
            LIST_PUSH(variables, scope->variables->entries[i].value);
        }
    }
    return variables;
}

static struct BoundScopeVariables* bound_scope_variables_new(void)
{
    struct BoundScopeVariables* variables = mc_malloc(sizeof(struct BoundScopeVariables));
    variables->num_entries = INITIAL_SCOPE_SIZE;
    variables->entries = mc_malloc(variables->num_entries * sizeof(struct BoundScopeVariableEntry));
    variables->count = 0;
    return variables;
}

static struct BoundScopeVariableEntry* bound_scope_variables_find_entry(struct BoundScopeVariableEntry* entries,
                                                                        long num_entries, sds name)
{
    if (num_entries == 0)
    {
        return NULL;
    }
    uint64_t hash = hash_pjw(name, sdslen(name));
    hash %= num_entries;
    while (entries[hash].name && sdscmp(entries[hash].name, name) != 0)
    {
        hash = (hash + 1) % num_entries;
    }
    return &entries[hash];
}
