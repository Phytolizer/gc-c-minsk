#pragma once

#include "Binding/BoundExpression.h"

struct Evaluator
{
  struct BoundExpression* root;
};

struct Evaluator* evaluator_new(struct BoundExpression* root);
struct Object* evaluator_evaluate(struct Evaluator* evaluator);
