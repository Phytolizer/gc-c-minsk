#pragma once

#include <common/VariableStore.h>

#include "Binding/BoundExpression.h"

struct Evaluator
{
  struct BoundExpression* root;
  struct VariableStore* variables;
};

struct Evaluator* evaluator_new(
    struct BoundExpression* root,
    struct VariableStore* variables);
struct Object* evaluator_evaluate(struct Evaluator* evaluator);
