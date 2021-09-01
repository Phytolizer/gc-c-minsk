#pragma once

#include "Syntax/ExpressionSyntax.h"

struct Evaluator
{
  struct ExpressionSyntax* root;
};

struct Evaluator* evaluator_new(struct ExpressionSyntax* root);
int evaluator_evaluate(struct Evaluator* evaluator);
