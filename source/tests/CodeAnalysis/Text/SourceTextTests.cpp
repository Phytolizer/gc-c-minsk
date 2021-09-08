#include <array>
#include <doctest/doctest.h>
#include <string>
extern "C"
{
#include <minsk/CodeAnalysis/Text/SourceText.h>
}

struct SourceTextIncludesLastLineTest
{
    std::string text;
    int expected_line_count;
};

const std::array SOURCE_TEXT_INCLUDES_LAST_LINE_TESTS = {
    SourceTextIncludesLastLineTest{".", 1},
    SourceTextIncludesLastLineTest{".\r\n", 2},
    SourceTextIncludesLastLineTest{".\r\n\r\n", 3},
};

TEST_SUITE("SourceText")
{
    TEST_CASE("includes last line")
    {
        for (auto& test : SOURCE_TEXT_INCLUDES_LAST_LINE_TESTS)
        {
            auto* source_text = source_text_from(sdsnew(test.text.c_str()));
            CHECK(source_text->lines->length == test.expected_line_count);
        }
    }
}
