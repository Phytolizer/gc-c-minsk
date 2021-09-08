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

DECLARE_NAMED_LIST(SyntaxKindList, SyntaxKind);

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
  if ((t1kind == SYNTAX_KIND_LESS_TOKEN || t1kind == SYNTAX_KIND_GREATER_TOKEN)
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

TEST_SUITE("Lexer")
{
  TEST_CASE("tests all token types")
  {
    auto* token_kinds = (SyntaxKindList*)mc_malloc(sizeof(SyntaxKindList));
    LIST_INIT(token_kinds);
    for (long i = 0; i < NUM_SYNTAX_KIND_VARIANTS; ++i)
    {
      std::string kind = SYNTAX_KINDS[i];
      if (kind.ends_with("KEYWORD") || kind.ends_with("TOKEN"))
      {
        LIST_PUSH(token_kinds, SYNTAX_KIND_VARIANTS[i]);
      }
    }
    auto* tested_token_kinds
        = (SyntaxKindList*)mc_malloc(sizeof(SyntaxKindList));
    for (long i = 0; i < TOKENS->length; ++i)
    {
      LIST_PUSH(tested_token_kinds, TOKENS->data[i].kind);
    }
    for (long i = 0; i < SEPARATORS.size(); ++i)
    {
      LIST_PUSH(tested_token_kinds, SEPARATORS[i].kind);
    }
    auto* untested_token_kinds
        = (SyntaxKindList*)mc_malloc(sizeof(SyntaxKindList));
    for (long i = 0; i < token_kinds->length; ++i)
    {
      bool tested = false;
      for (long j = 0; j < tested_token_kinds->length; ++j)
      {
        if (token_kinds->data[i] == tested_token_kinds->data[j])
        {
          tested = true;
          break;
        }
      }
      if (!tested && token_kinds->data[i] != SYNTAX_KIND_BAD_TOKEN
          && token_kinds->data[i] != SYNTAX_KIND_END_OF_FILE_TOKEN)
      {
        LIST_PUSH(untested_token_kinds, token_kinds->data[i]);
      }
    }

    CHECK(untested_token_kinds->length == 0);
    for (long i = 0; i < untested_token_kinds->length; ++i)
    {
      std::cerr << "untested: " << SYNTAX_KINDS[untested_token_kinds->data[i]]
                << std::endl;
    }
  }

  TEST_CASE("lexes token")
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

  TEST_CASE("lexes token pairs")
  {
    for (auto& test : get_token_pairs())
    {
      sds text
          = sdscatfmt(sdsempty(), "%S%S", test.first.text, test.second.text);
      auto* tokens = syntax_tree_parse_tokens(text);

      REQUIRE(tokens->length == 2);

      CHECK(
          std::string{SYNTAX_KINDS[tokens->data[0]->kind]}
          == std::string{SYNTAX_KINDS[test.first.kind]});
      CHECK(std::string{tokens->data[0]->text} == std::string{test.first.text});

      CHECK(
          std::string{SYNTAX_KINDS[tokens->data[1]->kind]}
          == std::string{SYNTAX_KINDS[test.second.kind]});
      CHECK(
          std::string{tokens->data[1]->text} == std::string{test.second.text});
    }
  }

  TEST_CASE("lexes token pairs with separator")
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
          std::string{tokens->data[1]->text}
          == std::string{test.separator.text});

      REQUIRE(tokens->length > 2);

      CHECK(
          std::string{SYNTAX_KINDS[tokens->data[2]->kind]}
          == std::string{SYNTAX_KINDS[test.t2.kind]});
      CHECK(std::string{tokens->data[2]->text} == std::string{test.t2.text});

      CHECK(tokens->length == 3);
    }
  }
}
