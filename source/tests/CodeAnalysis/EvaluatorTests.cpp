#include <array>
#include <doctest/doctest.h>
#include <iostream>
#include <string>

#include "AnnotatedText.hpp"

extern "C"
{
#include <common/Object.h>
#include <minsk/CodeAnalysis/Compilation.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <minsk/CodeAnalysis/VariableStore.h>
}

static void assert_value(const std::string& text, Object* expected_value);
static void assert_diagnostics(const std::string& text, const std::string& diagnostic_text);

struct EvaluatorTest
{
    std::string text;
    Object* expected_value;
};

const std::array EVALUATOR_TESTS = {
    EvaluatorTest{"1", OBJECT_INTEGER(1)},
    EvaluatorTest{"+1", OBJECT_INTEGER(1)},
    EvaluatorTest{"-1", OBJECT_INTEGER(-1)},
    EvaluatorTest{"~1", OBJECT_INTEGER(-2)},
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
    EvaluatorTest{"3 < 4", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"5 < 4", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"4 <= 4", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"4 <= 5", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"5 <= 4", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"3 > 4", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"5 > 4", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"4 >= 4", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"4 >= 5", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"5 >= 4", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"1 | 2", OBJECT_INTEGER(3)},
    EvaluatorTest{"1 | 0", OBJECT_INTEGER(1)},
    EvaluatorTest{"3 & 2", OBJECT_INTEGER(2)},
    EvaluatorTest{"2 & 1", OBJECT_INTEGER(0)},
    EvaluatorTest{"1 ^ 0", OBJECT_INTEGER(1)},
    EvaluatorTest{"0 ^ 1", OBJECT_INTEGER(1)},
    EvaluatorTest{"1 ^ 3", OBJECT_INTEGER(2)},
    EvaluatorTest{"true", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"!true", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"!false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"false == false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"true == false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"false != false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"true != false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"true && true", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"false || false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"true | false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"true | true", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"false & false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"true & false", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"false & true", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"true & true", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"false ^ true", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"true ^ false", OBJECT_BOOLEAN(true)},
    EvaluatorTest{"true ^ true", OBJECT_BOOLEAN(false)},
    EvaluatorTest{"{ var a = 0 (a = 10) + a }", OBJECT_INTEGER(20)},
    EvaluatorTest{"{ var a = 0 if a == 0 a = 10 a }", OBJECT_INTEGER(10)},
    EvaluatorTest{"{ var a = 0 if a == 5 a = 10 a }", OBJECT_INTEGER(0)},
    EvaluatorTest{
        "{ var a = 0 if a == 0 a = 10 else a = 5 a }",
        OBJECT_INTEGER(10),
    },
    EvaluatorTest{
        "{ var a = 0 if a == 5 a = 10 else a = 5 a }",
        OBJECT_INTEGER(5),
    },
    EvaluatorTest{
        "{ var i = 10 var result = 0 while i > 0 { result = result + i i = i - 1 } result }",
        OBJECT_INTEGER(55),
    },
    EvaluatorTest{
        "{ var result = 0 for i = 1 to 10 { result = result + i } result }",
        OBJECT_INTEGER(55),
    },
};

TEST_SUITE("Evaluator")
{
    TEST_CASE("computes correct values")
    {
        for (auto& test : EVALUATOR_TESTS)
        {
            assert_value(test.text, test.expected_value);
        }
    }

    TEST_CASE("variable declaration reports redeclaration")
    {
        std::string text = R"(
            {
                var x = 10
                var y = 100
                {
                    var x = 10
                }
                var [x] = 5
            }
        )";
        std::string diagnostics = R"(
            Variable 'x' is already declared.
        )";

        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("block statement: no infinite loop" * doctest::timeout(1))
    {
        std::string text = R"(
            {
            [)][]
        )";
        std::string diagnostics = R"(
            Unexpected token <CLOSE_PARENTHESIS_TOKEN>, expected <IDENTIFIER_TOKEN>.
            Unexpected token <END_OF_FILE_TOKEN>, expected <CLOSE_BRACE_TOKEN>.
        )";
        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("if statement reports cannot convert")
    {
        std::string text = R"(
            {
                var x = 0
                if [10]
                    x = 10
            }
        )";
        std::string diagnostics = R"(
            Cannot convert type 'INTEGER' to 'BOOLEAN'.
        )";
        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("for statement reports cannot convert lower bound")
    {
        std::string text = R"(
            {
                var result = 0
                for i = [false] to 10
                    result = result + i
            }
        )";
        std::string diagnostics = R"(
            Cannot convert type 'BOOLEAN' to 'INTEGER'.
        )";
        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("for statement reports cannot convert lower bound")
    {
        std::string text = R"(
            {
                var result = 0
                for i = 0 to [true]
                    result = result + i
            }
        )";
        std::string diagnostics = R"(
            Cannot convert type 'BOOLEAN' to 'INTEGER'.
        )";
        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("unary operator reports undefined")
    {
        std::string text = R"(
            [+]true
        )";
        std::string diagnostics = R"(
            The unary operator + is not defined for type 'BOOLEAN'.
        )";

        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("binary operator reports undefined")
    {
        std::string text = R"(
            8 [*] true
        )";
        std::string diagnostics = R"(
            The binary operator * is not defined for types 'INTEGER' and 'BOOLEAN'.
        )";

        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("name expression reports undefined")
    {
        std::string text = R"(
            [x] * 10
        )";
        std::string diagnostics = R"(
            Variable 'x' doesn't exist.
        )";

        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("name expression reports no error for inserted token")
    {
        std::string text = "[]";
        std::string diagnostics = R"(
            Unexpected token <END_OF_FILE_TOKEN>, expected <IDENTIFIER_TOKEN>.
        )";

        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("assignment expression reports undefined")
    {
        std::string text = R"(
            [x] = 10
        )";
        std::string diagnostics = R"(
            Variable 'x' doesn't exist.
        )";

        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("assignment expression reports cannot assign")
    {
        std::string text = R"(
            {
                let x = 10
                x [=] 11
            }
        )";
        std::string diagnostics = R"(
            Variable 'x' is read-only and cannot be assigned to.
        )";

        assert_diagnostics(text, diagnostics);
    }

    TEST_CASE("assignment expression reports cannot convert")
    {
        std::string text = R"(
            {
                var x = 10
                x = [true]
            }
        )";
        std::string diagnostics = R"(
            Cannot convert type 'BOOLEAN' to 'INTEGER'.
        )";

        assert_diagnostics(text, diagnostics);
    }
}

static void assert_value(const std::string& text_in, Object* expected_value)
{
    sds text = sdsnew(text_in.c_str());
    auto* tree = syntax_tree_parse(text);
    auto* compilation = compilation_new(tree);
    auto* variables = variable_store_new();
    auto* result = compilation_evaluate(compilation, variables);

    CHECK(result->diagnostics->length == 0);
    if (!objects_equal(expected_value, result->value))
    {
        std::cout << OBJECT_KINDS[result->value->kind] << " ";
        if (OBJECT_IS_INTEGER(result->value))
        {
            std::cout << OBJECT_AS_INTEGER(result->value)->value << " ";
        }
        std::cout << "!= " << OBJECT_KINDS[expected_value->kind] << " ";
        if (OBJECT_IS_INTEGER(expected_value))
        {
            std::cout << OBJECT_AS_INTEGER(expected_value)->value;
        }
        std::cout << std::endl;
        FAIL_CHECK("evaluation results not equal");
    }
}

static void assert_diagnostics(const std::string& text, const std::string& diagnostic_text)
{
    auto annotated_text = AnnotatedText::parse(text);
    auto syntax_tree = syntax_tree_parse(sdsnew(annotated_text.text.c_str()));
    auto compilation = compilation_new(syntax_tree);
    auto result = compilation_evaluate(compilation, variable_store_new());

    auto diagnostics = AnnotatedText::unindent_lines(diagnostic_text);

    if (annotated_text.spans.size() != diagnostics.size())
    {
        FAIL_CHECK("ERROR: Must mark as many spans as there are expected diagnostics");
    }

    CHECK(diagnostics.size() == result->diagnostics->length);

    for (int i = 0; i < diagnostics.size(); ++i)
    {
        auto expected_message = diagnostics[i];
        auto actual_message = result->diagnostics->data[i]->message;

        CHECK(expected_message == std::string{actual_message});

        auto expected_span = annotated_text.spans[i];
        auto actual_span = result->diagnostics->data[i]->span;

        CHECK(expected_span.start == actual_span->start);
        CHECK(expected_span.length == actual_span->length);
    }
}
