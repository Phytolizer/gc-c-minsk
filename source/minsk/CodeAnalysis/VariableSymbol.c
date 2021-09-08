#include "minsk/CodeAnalysis/VariableSymbol.h"

#include <limits.h>
#include <stdio.h>

#include <IncludeMe.h>
#include <common/Hashing.h>

struct VariableSymbol* variable_symbol_new(sds name, bool is_read_only, enum ObjectKind type)
{
    struct VariableSymbol* symbol = mc_malloc(sizeof(struct VariableSymbol));
    symbol->name = name;
    symbol->is_read_only = is_read_only;
    symbol->type = type;
    return symbol;
}

uint64_t variable_symbol_hash(const struct VariableSymbol* symbol)
{
    return hash_pjw(symbol, sizeof(struct VariableSymbol));
}
