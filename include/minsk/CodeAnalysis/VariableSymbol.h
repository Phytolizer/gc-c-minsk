#pragma once

#include <stdint.h>

#include <common/List.h>
#include <common/Object.h>
#include <sds.h>

struct VariableSymbol
{
  sds name;
  bool is_read_only;
  enum ObjectKind type;
};

DECLARE_NAMED_LIST(VariableSymbolList, struct VariableSymbol*);

struct VariableSymbol* variable_symbol_new(
    sds name,
    bool is_read_only,
    enum ObjectKind type);
uint64_t variable_symbol_hash(const struct VariableSymbol* symbol);
