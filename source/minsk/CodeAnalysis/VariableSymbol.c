#include "minsk/CodeAnalysis/VariableSymbol.h"

#include <limits.h>

#include <IncludeMe.h>

__attribute__((const)) inline static uint64_t murmur_mix64(uint64_t k)
{
  k ^= k >> 33;
  k *= 0xFF51AFD7ED558CCDULL;
  k ^= k >> 33;
  k *= 0xC4CEB9FE1A85EC53ULL;
  k ^= k >> 33;
  return k;
}

__attribute__((const)) inline static uint64_t murmur_rotl64(
    uint64_t x,
    uint64_t r)
{
  return (x << r) | (x >> (64 - r));
}

__attribute__((const)) inline static uint64_t* murmur_hash3_x64_128(
    const void* data_in,
    size_t len,
    const uint32_t seed)
{
  const uint8_t* data = (const uint8_t*)data_in;
#define BLKSIZE (2 * sizeof(uint64_t))
  const size_t nblocks = len / BLKSIZE;
  uint64_t h1 = seed;
  uint64_t h2 = seed;
#define C1 0x87C37B91114253D5ULL
#define C2 0x4CF5AD432745937FULL
  const uint64_t* blocks = (const uint64_t*)data;
  for (size_t i = 0; i < nblocks; ++i)
  {
    uint64_t k1 = blocks[i * 2];
    uint64_t k2 = blocks[i * 2 + 1];
    k1 *= C1;
    k1 = murmur_rotl64(k1, 31);
    k1 *= C2;
    h1 ^= k1;
    h1 = murmur_rotl64(h1, 27);
    h1 += h2;
    h1 = h1 * 5 + 0x52DCE729;
    k2 *= C2;
    k2 = murmur_rotl64(k2, 33);
    k2 *= C1;
    h2 ^= k2;
    h2 = murmur_rotl64(h2, 31);
    h2 += h1;
    h2 = h2 * 5 + 0x38495AB5;
  }
  const uint8_t* tail = (const uint8_t*)(data + nblocks * BLKSIZE);
  uint64_t k1 = 0;
  uint64_t k2 = 0;
  switch (len % BLKSIZE)
  {
    case 15:
      k2 ^= ((uint64_t)(tail[14])) << 48;
    case 14:
      k2 ^= ((uint64_t)(tail[13])) << 40;
    case 13:
      k2 ^= ((uint64_t)(tail[12])) << 32;
    case 12:
      k2 ^= ((uint64_t)(tail[11])) << 24;
    case 11:
      k2 ^= ((uint64_t)(tail[10])) << 16;
    case 10:
      k2 ^= ((uint64_t)(tail[9])) << 8;
    case 9:
      k2 ^= ((uint64_t)(tail[8]));
      k2 *= C2;
      k2 = murmur_rotl64(k2, 33);
      k2 *= C1;
      h2 ^= k2;
    case 8:
      k2 ^= ((uint64_t)(tail[7])) << 56;
    case 7:
      k2 ^= ((uint64_t)(tail[6])) << 48;
    case 6:
      k2 ^= ((uint64_t)(tail[5])) << 40;
    case 5:
      k2 ^= ((uint64_t)(tail[4])) << 32;
    case 4:
      k2 ^= ((uint64_t)(tail[3])) << 24;
    case 3:
      k2 ^= ((uint64_t)(tail[2])) << 16;
    case 2:
      k2 ^= ((uint64_t)(tail[1])) << 8;
    case 1:
      k2 ^= ((uint64_t)(tail[0]));
      k1 *= C1;
      k1 = murmur_rotl64(k1, 31);
      k1 *= C2;
      h1 ^= k1;
  }

  h1 ^= len;
  h2 ^= len;
  h1 += h2;
  h2 += h1;
  h1 = murmur_mix64(h1);
  h2 = murmur_mix64(h2);
  h1 += h2;
  h2 += h1;

  uint64_t* out = mc_malloc(BLKSIZE);
  out[0] = h1;
  out[1] = h2;
  return out;
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
  return murmur_hash3_x64_128(
      symbol,
      sizeof(struct VariableSymbol),
      symbol->type)[0];
}
