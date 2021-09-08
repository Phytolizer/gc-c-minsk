#pragma once

#include <minsk/CodeAnalysis/VariableStore.h>

#include "Binding/BoundStatement.h"

struct Evaluator
{
    struct BoundStatement* root;
    struct VariableStore* variables;
    struct Object* last_value;
};

struct Evaluator* evaluator_new(struct BoundStatement* root, struct VariableStore* variables);
struct Object* evaluator_evaluate(struct Evaluator* evaluator);
