#pragma once

#include "Binding/BoundExpression.h"

struct Evaluator
{
  struct BoundExpression* root;
};

struct Evaluator* evaluator_new(struct BoundExpression* root);
int evaluator_evaluate(struct Evaluator* evaluator);
