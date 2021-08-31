#pragma once

#include <gc.h>
#include <stdlib.h>

#define mc_malloc GC_MALLOC
#define mc_realloc GC_REALLOC
#define mc_free GC_FREE
