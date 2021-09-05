#include <algorithm>
#include <doctest/doctest.h>
#include <utility>
#include <vector>
extern "C" {
#include <minsk/CodeAnalysis/Syntax/SyntaxFacts.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
}

#include "AssertingEnumerator.hpp"

static std::vector<SyntaxKind> syntax_kind_variants()
{
  std::vector<SyntaxKind> syntax_kind_variants;
  std::copy(
      SYNTAX_KIND_VARIANTS,
      &SYNTAX_KIND_VARIANTS[NUM_SYNTAX_KIND_VARIANTS],
      std::back_inserter(syntax_kind_variants));
  return syntax_kind_variants;
}

static std::vector<SyntaxKind> get_unary_operator_kinds()
{
  std::vector<SyntaxKind> unary_operators;
  for (auto& kind : syntax_kind_variants())
  {
    if (unary_operator_precedence(kind) > 0)
    {
      unary_operators.emplace_back(kind);
    }
  }
  return unary_operators;
}

static std::vector<SyntaxKind> get_binary_operator_kinds()
{
  std::vector<SyntaxKind> binary_operators;
  for (auto& kind : syntax_kind_variants())
  {
    if (binary_operator_precedence(kind) > 0)
    {
      binary_operators.emplace_back(kind);
    }
  }
  return binary_operators;
}

static std::vector<std::pair<SyntaxKind, SyntaxKind>>
    get_binary_operator_pairs()
{
  std::vector<std::pair<SyntaxKind, SyntaxKind>> binary_operator_pairs;
  for (auto k1 : get_binary_operator_kinds())
  {
    for (auto k2 : get_binary_operator_kinds())
    {
      binary_operator_pairs.emplace_back(k1, k2);
    }
  }
  return binary_operator_pairs;
}

TEST_CASE("parser: binary expression honors precedences")
{
  for (auto& test : get_binary_operator_pairs())
  {
    int op1_precedence = binary_operator_precedence(test.first);
    int op2_precedence = binary_operator_precedence(test.second);
    sds op1_text = syntax_facts_get_text(test.first);
    sds op2_text = syntax_facts_get_text(test.second);
    sds text = sdscatfmt(sdsempty(), "a %S b %S c", op1_text, op2_text);
    SyntaxNode* expression
        = reinterpret_cast<SyntaxNode*>(syntax_tree_parse(text)->root);

    if (op1_precedence >= op2_precedence)
    {
      AssertingEnumerator e{expression};
      e.assert_node(SYNTAX_KIND_BINARY_EXPRESSION);
      e.assert_node(SYNTAX_KIND_BINARY_EXPRESSION);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "a");
      e.assert_token(test.first, op1_text);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "b");
      e.assert_token(test.second, op2_text);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "c");
    }
    else
    {
      AssertingEnumerator e{expression};
      e.assert_node(SYNTAX_KIND_BINARY_EXPRESSION);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "a");
      e.assert_token(test.first, op1_text);
      e.assert_node(SYNTAX_KIND_BINARY_EXPRESSION);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "b");
      e.assert_token(test.second, op2_text);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "c");
    }
  }
}

static std::vector<std::pair<SyntaxKind, SyntaxKind>> get_unary_operator_pairs()
{
  std::vector<std::pair<SyntaxKind, SyntaxKind>> binary_operator_pairs;
  for (auto k1 : get_unary_operator_kinds())
  {
    for (auto k2 : get_binary_operator_kinds())
    {
      binary_operator_pairs.emplace_back(k1, k2);
    }
  }
  return binary_operator_pairs;
}

TEST_CASE("parser: unary expression honors precedences")
{
  for (auto& test : get_unary_operator_pairs())
  {
    int unary_precedence = unary_operator_precedence(test.first);
    int binary_precedence = binary_operator_precedence(test.second);
    sds unary_text = syntax_facts_get_text(test.first);
    sds binary_text = syntax_facts_get_text(test.second);
    sds text = sdscatfmt(sdsempty(), "%S a %S b", unary_text, binary_text);
    SyntaxNode* expression
        = reinterpret_cast<SyntaxNode*>(syntax_tree_parse(text)->root);

    if (unary_precedence >= binary_precedence)
    {
      AssertingEnumerator e{expression};
      e.assert_node(SYNTAX_KIND_BINARY_EXPRESSION);
      e.assert_node(SYNTAX_KIND_UNARY_EXPRESSION);
      e.assert_token(test.first, unary_text);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "a");
      e.assert_token(test.second, binary_text);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "b");
    }
    else
    {
      AssertingEnumerator e{expression};
      e.assert_node(SYNTAX_KIND_UNARY_EXPRESSION);
      e.assert_token(test.first, unary_text);
      e.assert_node(SYNTAX_KIND_BINARY_EXPRESSION);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "a");
      e.assert_token(test.second, binary_text);
      e.assert_node(SYNTAX_KIND_NAME_EXPRESSION);
      e.assert_token(SYNTAX_KIND_IDENTIFIER_TOKEN, "b");
    }
  }
}
