#include "minsk/CodeAnalysis/VariableSymbol.h"

#include <limits.h>
#include <stdio.h>

#include <IncludeMe.h>

#define HIGH 0xF000000000000000UL

static uint64_t hash_pjw(const void* p, size_t size)
{
  uint64_t hash = 0;
  const uint8_t* data = p;
  for (size_t i = 0; i < size; ++i)
  {
    hash = (hash << 4) + data[i];
    uint64_t high = hash & HIGH;
    if (high)
    {
      hash ^= high >> 56;
    }
    hash &= ~high;
  }
  return hash;
}

struct VariableSymbol* variable_symbol_new(sds name, enum ObjectKind type)
{
  struct VariableSymbol* symbol = mc_malloc(sizeof(struct VariableSymbol));
  symbol->name = name;
  symbol->type = type;
  return symbol;
}

uint64_t variable_symbol_hash(const struct VariableSymbol* symbol)
{
  return hash_pjw(symbol, sizeof(struct VariableSymbol));
}
