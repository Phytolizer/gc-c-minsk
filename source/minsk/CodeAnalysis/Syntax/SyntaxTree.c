#include "minsk/CodeAnalysis/Syntax/SyntaxTree.h"

#include "Parser.h"

struct SyntaxTree* syntax_tree_new(
    struct StringList* diagnostics,
    struct ExpressionSyntax* root,
    struct SyntaxToken* end_of_file_token)
{
  struct SyntaxTree* tree = mc_malloc(sizeof(struct SyntaxTree));
  tree->diagnostics = diagnostics;
  tree->root = root;
  tree->end_of_file_token = end_of_file_token;
  return tree;
}

struct SyntaxTree* syntax_tree_parse(sds text)
{
  struct Parser* parser = parser_new(text);
  return parser_parse(parser);
}
