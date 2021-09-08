#include <doctest/doctest.h>
#include <iterator>
#include <vector>
extern "C"
{
#include <minsk/CodeAnalysis/Syntax/SyntaxFacts.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
}

TEST_SUITE("SyntaxFacts")
{
    TEST_CASE("syntax_facts_get_text round trips")
    {
        std::vector<SyntaxKind> syntax_kind_variants;
        std::copy(SYNTAX_KIND_VARIANTS, &SYNTAX_KIND_VARIANTS[NUM_SYNTAX_KIND_VARIANTS],
                  std::back_inserter(syntax_kind_variants));
        for (auto &value : syntax_kind_variants)
        {
            sds text = syntax_facts_get_text(value);
            if (text == NULL)
            {
                continue;
            }

            SyntaxTokenList *tokens = syntax_tree_parse_tokens(text);
            REQUIRE(tokens->length == 1);
            CHECK(tokens->data[0]->kind == value);
            CHECK(std::string{tokens->data[0]->text} == std::string{text});
        }
    }
}
