#include <algorithm>
#include <array>
#include <doctest/doctest.h>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "minsk/CodeAnalysis/Syntax/SyntaxKind.h"

extern "C" {
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
}

struct LexerLexesTokenTest
{
  SyntaxKind kind;
  std::string text;
};

const std::array TOKENS = {
    LexerLexesTokenTest{SYNTAX_KIND_IDENTIFIER_TOKEN, "a"},
    LexerLexesTokenTest{SYNTAX_KIND_IDENTIFIER_TOKEN, "abc"},
    LexerLexesTokenTest{SYNTAX_KIND_PLUS_TOKEN, "+"},
    LexerLexesTokenTest{SYNTAX_KIND_MINUS_TOKEN, "-"},
    LexerLexesTokenTest{SYNTAX_KIND_STAR_TOKEN, "*"},
    LexerLexesTokenTest{SYNTAX_KIND_SLASH_TOKEN, "/"},
    LexerLexesTokenTest{SYNTAX_KIND_BANG_TOKEN, "!"},
    LexerLexesTokenTest{SYNTAX_KIND_EQUALS_TOKEN, "="},
    LexerLexesTokenTest{SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN, "&&"},
    LexerLexesTokenTest{SYNTAX_KIND_PIPE_PIPE_TOKEN, "||"},
    LexerLexesTokenTest{SYNTAX_KIND_EQUALS_EQUALS_TOKEN, "=="},
    LexerLexesTokenTest{SYNTAX_KIND_BANG_EQUALS_TOKEN, "!="},
    LexerLexesTokenTest{SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN, "("},
    LexerLexesTokenTest{SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN, ")"},
    LexerLexesTokenTest{SYNTAX_KIND_FALSE_KEYWORD, "false"},
    LexerLexesTokenTest{SYNTAX_KIND_TRUE_KEYWORD, "true"},
};

const std::array SEPARATORS = {
    LexerLexesTokenTest{SYNTAX_KIND_WHITESPACE_TOKEN, " "},
    LexerLexesTokenTest{SYNTAX_KIND_WHITESPACE_TOKEN, "  "},
    LexerLexesTokenTest{SYNTAX_KIND_WHITESPACE_TOKEN, "\r"},
    LexerLexesTokenTest{SYNTAX_KIND_WHITESPACE_TOKEN, "\n"},
    LexerLexesTokenTest{SYNTAX_KIND_WHITESPACE_TOKEN, "\r\n"},
};

TEST_CASE("lexer lexes token")
{
  std::vector<LexerLexesTokenTest> tests;
  std::copy(TOKENS.begin(), TOKENS.end(), std::back_inserter(tests));
  std::copy(SEPARATORS.begin(), SEPARATORS.end(), std::back_inserter(tests));

  for (auto& test : tests)
  {
    SyntaxTokenList* tokens
        = syntax_tree_parse_tokens(sdsnew(test.text.c_str()));
    CHECK(tokens->length == 1);
    SyntaxToken* token = tokens->data[0];
    CHECK(token->kind == test.kind);
    CHECK(std::string{token->text} == test.text);
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

static std::vector<std::pair<LexerLexesTokenTest, LexerLexesTokenTest>>
    get_token_pairs()
{
  std::vector<std::pair<LexerLexesTokenTest, LexerLexesTokenTest>> tests;
  for (auto& t1 : TOKENS)
  {
    for (auto& t2 : TOKENS)
    {
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
  LexerLexesTokenTest t1;
  LexerLexesTokenTest separator;
  LexerLexesTokenTest t2;
};

static std::vector<TokenPairWithSeparator> get_token_pairs_with_separator()
{
  std::vector<TokenPairWithSeparator> tests;
  for (auto& t1 : TOKENS)
  {
    for (auto& t2 : TOKENS)
    {
      if (requires_separator(t1.kind, t2.kind))
      {
        for (auto& separator : SEPARATORS)
        {
          tests.emplace_back(t1, separator, t2);
        }
      }
    }
  }
  return tests;
}

TEST_CASE("lexer lexes token pairs")
{
  for (auto& test : get_token_pairs())
  {
#if 0
    std::cout << "\nt1 kind: " << SYNTAX_KINDS[test.first.kind];
    std::cout << "\nt1 text: '" << test.first.text << "'";
    std::cout << "\nt2 kind: " << SYNTAX_KINDS[test.second.kind];
    std::cout << "\nt2 text: '" << test.second.text << "'";
    std::cout << "\nconcatenated: '" << test.first.text << test.second.text
              << "'";
    std::cout << "\n";
#endif
    sds text = sdscatfmt(
        sdsempty(),
        "%s%s",
        test.first.text.c_str(),
        test.second.text.c_str());
    auto* tokens = syntax_tree_parse_tokens(text);

    REQUIRE(tokens->length == 2);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[0]->kind]}
        == std::string{SYNTAX_KINDS[test.first.kind]});
    CHECK(tokens->data[0]->text == test.first.text);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[1]->kind]}
        == std::string{SYNTAX_KINDS[test.second.kind]});
    CHECK(tokens->data[1]->text == test.second.text);
  }
}

TEST_CASE("lexer lexes token pairs with separator")
{
  for (auto& test : get_token_pairs_with_separator())
  {
    sds text = sdscatfmt(
        sdsempty(),
        "%s%s%s",
        test.t1.text.c_str(),
        test.separator.text.c_str(),
        test.t2.text.c_str());
    auto* tokens = syntax_tree_parse_tokens(text);

    REQUIRE(tokens->length == 3);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[0]->kind]}
        == std::string{SYNTAX_KINDS[test.t1.kind]});
    CHECK(tokens->data[0]->text == test.t1.text);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[1]->kind]}
        == std::string{SYNTAX_KINDS[test.separator.kind]});
    CHECK(tokens->data[1]->text == test.separator.text);

    CHECK(
        std::string{SYNTAX_KINDS[tokens->data[2]->kind]}
        == std::string{SYNTAX_KINDS[test.t2.kind]});
    CHECK(tokens->data[2]->text == test.t2.text);
  }
}
