#pragma once

#include <doctest/doctest.h>
#include <stack>
#include <string>
#include <vector>
extern "C"
{
#include <minsk/CodeAnalysis/Syntax/SyntaxNode.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxToken.h>
}

class AssertingEnumerator
{
    static std::vector<SyntaxNode *> flatten(SyntaxNode *root);
    std::vector<SyntaxNode *> flattened;
    std::vector<SyntaxNode *>::iterator enumerator;

  public:
    AssertingEnumerator(SyntaxNode *root);
    ~AssertingEnumerator();

    void assert_token(SyntaxKind kind, std::string text);
    void assert_node(SyntaxKind kind);
};

std::vector<SyntaxNode *> AssertingEnumerator::flatten(SyntaxNode *root)
{
    std::stack<SyntaxNode *> stack;
    std::vector<SyntaxNode *> result;
    stack.push(root);

    while (!stack.empty())
    {
        SyntaxNode *n = stack.top();
        stack.pop();
        result.push_back(n);

        SyntaxNodeList *children = syntax_node_get_children(n);
        for (long i = children->length - 1; i >= 0; --i)
        {
            stack.push(children->data[i]);
        }
    }
    return result;
}

AssertingEnumerator::AssertingEnumerator(SyntaxNode *root) : flattened(flatten(root)), enumerator(flattened.begin())
{
}

AssertingEnumerator::~AssertingEnumerator()
{
    CHECK(enumerator == flattened.end());
}

void AssertingEnumerator::assert_token(SyntaxKind kind, std::string text)
{
    REQUIRE(enumerator != flattened.end());
    REQUIRE((*enumerator)->kind == SYNTAX_NODE_KIND_TOKEN);
    SyntaxToken *token = reinterpret_cast<SyntaxToken *>(*enumerator);
    CHECK(token->kind == kind);
    CHECK(std::string{token->text} == text);
    ++enumerator;
}

void AssertingEnumerator::assert_node(SyntaxKind kind)
{
    REQUIRE(enumerator != flattened.end());
    CHECK((*enumerator)->kind != SYNTAX_NODE_KIND_TOKEN);
    CHECK(std::string{SYNTAX_KINDS[syntax_node_get_kind(*enumerator)]} == std::string{SYNTAX_KINDS[kind]});
    ++enumerator;
}
