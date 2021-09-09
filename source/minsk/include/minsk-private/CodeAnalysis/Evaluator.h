#pragma once

#include <minsk-private/CodeAnalysis/Binding/BoundStatement.h>
#include <minsk/CodeAnalysis/VariableStore.h>

struct Evaluator
{
    struct BoundStatement* root;
    struct VariableStore* variables;
    struct Object* last_value;
};

struct Evaluator* evaluator_new(struct BoundStatement* root, struct VariableStore* variables);
struct Object* evaluator_evaluate(struct Evaluator* evaluator);
