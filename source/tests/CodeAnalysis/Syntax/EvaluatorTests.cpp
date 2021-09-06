#include <array>
#include <doctest/doctest.h>
#include <iostream>
#include <string>

extern "C" {
#include <common/Object.h>
#include <minsk/CodeAnalysis/Compilation.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <minsk/CodeAnalysis/VariableStore.h>
}

struct EvaluatorTest
{
  std::string text;
  Object* expected_value;
};

const std::array EVALUATOR_TESTS = {
    EvaluatorTest{"1", OBJECT_INTEGER(1)},
    EvaluatorTest{"+1", OBJECT_INTEGER(1)},
    EvaluatorTest{"-1", OBJECT_INTEGER(-1)},
    EvaluatorTest{"14 + 12", OBJECT_INTEGER(26)},
    EvaluatorTest{"12 - 3", OBJECT_INTEGER(9)},
    EvaluatorTest{"4 * 2", OBJECT_INTEGER(8)},
    EvaluatorTest{"9 / 3", OBJECT_INTEGER(3)},
    EvaluatorTest{"(10)", OBJECT_INTEGER(10)},
    EvaluatorTest{"12 == 3", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"3 == 3", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"12 != 3", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"3 != 3", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"false == false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"true == false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"false != false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"true != false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"true", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"!true", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"!false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"(a = 10) + a", OBJECT_INTEGER(20)},
};

TEST_SUITE("Evaluator")
{
  TEST_CASE("sanity check")
  {
    for (auto& test : EVALUATOR_TESTS)
    {
      sds text = sdsnew(test.text.c_str());
      auto* tree = syntax_tree_parse(text);
      auto* compilation = compilation_new(tree);
      auto* variables = variable_store_new();
      variable_store_insert_or_assign(
          variables,
          variable_symbol_new(sdsnew("a"), OBJECT_KIND_INTEGER),
          OBJECT_INTEGER(42));
      auto* result = compilation_evaluate(compilation, variables);

      CHECK(result->diagnostics->length == 0);
      // std::cout << OBJECT_KINDS[result->value->kind] << " ";
      // if (OBJECT_IS_INTEGER(result->value))
      // {
      //   std::cout << OBJECT_AS_INTEGER(result->value)->value << " ";
      // }
      // std::cout << "== " << OBJECT_KINDS[test.expected_value->kind] << " ";
      // if (OBJECT_IS_INTEGER(test.expected_value))
      // {
      //   std::cout << OBJECT_AS_INTEGER(test.expected_value)->value;
      // }
      // std::cout << std::endl;
      CHECK(objects_equal(test.expected_value, result->value));
    }
  }
}
