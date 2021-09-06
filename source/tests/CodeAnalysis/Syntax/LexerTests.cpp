#include <algorithm>
#include <array>
#include <doctest/doctest.h>
#include <gc.h>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

extern "C" {
#include <common/List.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxFacts.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
}

struct BasicToken
{
  SyntaxKind kind;
  sds text;
};

DECLARE_NAMED_LIST(BasicTokenList, BasicToken);

BasicTokenList* get_dynamic_tokens()
{
  BasicTokenList* dynamic_tokens
      = static_cast<BasicTokenList*>(mc_malloc(sizeof(BasicTokenList)));
  LIST_INIT(dynamic_tokens);
  LIST_PUSH(
      dynamic_tokens,
      ((BasicToken){SYNTAX_KIND_NUMBER_TOKEN, sdsnew("1")}));
  LIST_PUSH(
      dynamic_tokens,
      ((BasicToken){SYNTAX_KIND_NUMBER_TOKEN, sdsnew("123")}));
  LIST_PUSH(
      dynamic_tokens,
      ((BasicToken){SYNTAX_KIND_IDENTIFIER_TOKEN, sdsnew("a")}));
  LIST_PUSH(
      dynamic_tokens,
      ((BasicToken){SYNTAX_KIND_IDENTIFIER_TOKEN, sdsnew("abc")}));
  return dynamic_tokens;
}

const BasicTokenList* DYNAMIC_TOKENS = get_dynamic_tokens();

BasicTokenList* get_tokens()
{
  BasicTokenList* fixed_tokens
      = static_cast<BasicTokenList*>(mc_malloc(sizeof(BasicTokenList)));
  LIST_INIT(fixed_tokens);
  LIST_RESERVE(fixed_tokens, NUM_SYNTAX_KIND_VARIANTS);
  std::transform(
      SYNTAX_KIND_VARIANTS,
      &SYNTAX_KIND_VARIANTS[NUM_SYNTAX_KIND_VARIANTS],
      fixed_tokens->data,
      [](SyntaxKind kind) {
        return BasicToken{kind, syntax_facts_get_text(kind)};
      });
  fixed_tokens->length = NUM_SYNTAX_KIND_VARIANTS;
  BasicTokenList* tokens
      = static_cast<BasicTokenList*>(mc_malloc(sizeof(BasicTokenList)));
  LIST_INIT(tokens);
  LIST_RESERVE(tokens, NUM_SYNTAX_KIND_VARIANTS + DYNAMIC_TOKENS->length);
  auto* ep = std::copy_if(
      fixed_tokens->data,
      &fixed_tokens->data[fixed_tokens->length],
      tokens->data,
      [](const auto& t) { return t.text != nullptr; });
  tokens->length = ep - tokens->data;
  ep = std::copy(
      DYNAMIC_TOKENS->data,
      &DYNAMIC_TOKENS->data[DYNAMIC_TOKENS->length],
      &tokens->data[tokens->length]);
  tokens->length = ep - tokens->data;
  return tokens;
}

BasicTokenList* TOKENS = get_tokens();

const std::array SEPARATORS = {
    BasicToken{SYNTAX_KIND_WHITESPACE_TOKEN, sdsnew(" ")},
    BasicToken{SYNTAX_KIND_WHITESPACE_TOKEN, sdsnew("  ")},
    BasicToken{SYNTAX_KIND_WHITESPACE_TOKEN, sdsnew("\r")},
    BasicToken{SYNTAX_KIND_WHITESPACE_TOKEN, sdsnew("\n")},
    BasicToken{SYNTAX_KIND_WHITESPACE_TOKEN, sdsnew("\r\n")},
};

TEST_CASE("lexer lexes token")
{
  std::vector<BasicToken> tests;
  std::copy(
      TOKENS->data,
      &TOKENS->data[TOKENS->length],
      std::back_inserter(tests));
  std::copy(SEPARATORS.begin(), SEPARATORS.end(), std::back_inserter(tests));

  for (auto& test : tests)
  {
    SyntaxTokenList* tokens = syntax_tree_parse_tokens(test.text);
    CHECK(tokens->length == 1);
    SyntaxToken* token = tokens->data[0];
    CHECK(
        std::string{SYNTAX_KINDS[token->kind]}
        == std::string{SYNTAX_KINDS[test.kind]});
    CHECK(std::string{token->text} == std::string{test.text});
  }
}

static inline bool requires_separator(SyntaxKind t1kind, SyntaxKind t2kind)
{
  bool t1_is_keyword = std::string{SYNTAX_KINDS[t1kind]}.ends_with("KEYWORD");
  bool t2_is_keyword = std::string{SYNTAX_KINDS[t2kind]}.ends_with("KEYWORD");
  if (t1kind == SYNTAX_KIND_IDENTIFIER_TOKEN
      && t2kind == SYNTAX_KIND_IDENTIFIER_TOKEN)
  {
    return true;
  }
  if (t1kind == SYNTAX_KIND_NUMBER_TOKEN && t2kind == SYNTAX_KIND_NUMBER_TOKEN)
  {
    return true;
  }
  if (t1_is_keyword && t2_is_keyword)
  {
    return true;
  }
  if (t1_is_keyword && t2kind == SYNTAX_KIND_IDENTIFIER_TOKEN)
  {
    return true;
  }
  if (t1kind == SYNTAX_KIND_IDENTIFIER_TOKEN && t2_is_keyword)
  {
    return true;
  }
  if (t1kind == SYNTAX_KIND_BANG_TOKEN
      && (t2kind == SYNTAX_KIND_EQUALS_TOKEN
          || t2kind == SYNTAX_KIND_EQUALS_EQUALS_TOKEN))
  {
    return true;
  }
  if (t1kind == SYNTAX_KIND_EQUALS_TOKEN
      && (t2kind == SYNTAX_KIND_EQUALS_TOKEN
          || t2kind == SYNTAX_KIND_EQUALS_EQUALS_TOKEN))
  {
    return true;
  }
  return false;
}

static std::vector<std::pair<BasicToken, BasicToken>> get_token_pairs()
{
  std::vector<std::pair<BasicToken, BasicToken>> tests;
  for (long i = 0; i < TOKENS->length; ++i)
  {
    for (long j = 0; j < TOKENS->length; ++j)
    {
      auto& t1 = TOKENS->data[i];
      auto& t2 = TOKENS->data[j];
      if (!requires_separator(t1.kind, t2.kind))
      {
        tests.emplace_back(t1, t2);
      }
    }
  }
  return tests;
}

struct TokenPairWithSeparator
{
  BasicToken t1;
  BasicToken separator;
  BasicToken t2;
};

DECLARE_NAMED_LIST(TokenPairWithSeparatorList, TokenPairWithSeparator);

static TokenPairWithSeparatorList* get_token_pairs_with_separator()
{
  TokenPairWithSeparatorList* tests = static_cast<TokenPairWithSeparatorList*>(
      mc_malloc(sizeof(TokenPairWithSeparatorList)));
  LIST_INIT(tests);
  for (long i = 0; i < TOKENS->length; ++i)
  {
    for (long j = 0; j < TOKENS->length; ++j)
    {
      auto& t1 = TOKENS->data[i];
      auto& t2 = TOKENS->data[j];
      if (requires_separator(t1.kind, t2.kind))
      {
        for (auto& separator : SEPARATORS)
        {
          LIST_PUSH(tests, ((TokenPairWithSeparator){t1, separator, t2}));
        }
      }
    }
  }
  return tests;
}

const TokenPairWithSeparatorList* TOKEN_PAIRS_WITH_SEPARATOR
    = get_token_pairs_with_separator();

TEST_CASE("lexer lexes token pairs")
{
  for (auto& test : get_token_pairs())
  {
    // std::cout << "\nt1 kind: " << SYNTAX_KINDS[test.first.kind];
    // std::cout << "\nt1 text: '" << test.first.text << "'";
    // std::cout << "\nt2 kind: " << SYNTAX_KINDS[test.second.kind];
    // std::cout << "\nt2 text: '" << test.second.text << "'";
    // std::cout << "\nconcatenated: '" << test.first.text << test.second.text
    //           << "'";
    // std::cout << "\n";
    sds text = sdscatfmt(sdsempty(), "%S%S", test.first.text, test.second.text);
    auto* tokens = syntax_tree_parse_tokens(text);

    REQUIRE(tokens->length == 2);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[0]->kind]}
        == std::string{SYNTAX_KINDS[test.first.kind]});
    CHECK(std::string{tokens->data[0]->text} == std::string{test.first.text});

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[1]->kind]}
        == std::string{SYNTAX_KINDS[test.second.kind]});
    CHECK(std::string{tokens->data[1]->text} == std::string{test.second.text});
  }
}

TEST_CASE("lexer lexes token pairs with separator")
{
  for (long i = 0; i < TOKEN_PAIRS_WITH_SEPARATOR->length; ++i)
  {
    auto& test = TOKEN_PAIRS_WITH_SEPARATOR->data[i];
    sds text = sdscatfmt(
        sdsempty(),
        "%S%S%S",
        test.t1.text,
        test.separator.text,
        test.t2.text);
    auto* tokens = syntax_tree_parse_tokens(text);

    REQUIRE(tokens->length > 0);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[0]->kind]}
        == std::string{SYNTAX_KINDS[test.t1.kind]});
    CHECK(std::string{tokens->data[0]->text} == std::string{test.t1.text});

    REQUIRE(tokens->length > 1);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[1]->kind]}
        == std::string{SYNTAX_KINDS[test.separator.kind]});
    CHECK(
        std::string{tokens->data[1]->text} == std::string{test.separator.text});

    REQUIRE(tokens->length > 2);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[2]->kind]}
        == std::string{SYNTAX_KINDS[test.t2.kind]});
    CHECK(std::string{tokens->data[2]->text} == std::string{test.t2.text});

    CHECK(tokens->length == 3);
  }
}
