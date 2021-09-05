#pragma once

#include <stdint.h>

#include <common/Object.h>
#include <sds.h>

struct VariableSymbol
{
  sds name;
  enum ObjectKind type;
};

struct VariableSymbol* variable_symbol_new(sds name, enum ObjectKind type);
uint64_t variable_symbol_hash(const struct VariableSymbol* symbol);
