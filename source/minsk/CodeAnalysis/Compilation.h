#pragma once

#include <minsk/CodeAnalysis/Compilation.h>

#include "Binding/BoundGlobalScope.h"

struct BoundGlobalScope* compilation_get_global_scope(struct Compilation* compilation);
