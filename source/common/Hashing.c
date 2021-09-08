#include "common/Hashing.h"

#define HIGH 0xF000000000000000UL

uint64_t hash_pjw(const void* p, size_t size)
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
