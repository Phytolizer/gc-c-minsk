#include "minsk/CodeAnalysis/Syntax/SyntaxTree.h"

#include "Lexer.h"
#include "Parser.h"

struct SyntaxTree* syntax_tree_new(
    struct DiagnosticBag* diagnostics,
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

struct SyntaxTokenList* syntax_tree_parse_tokens(const sds text)
{
  struct SyntaxTokenList* list = mc_malloc(sizeof(struct SyntaxTokenList));
  struct Lexer* lexer = lexer_new(text);
  while (true)
  {
    struct SyntaxToken* token = lexer_next_token(lexer);
    if (token->kind == SYNTAX_KIND_END_OF_FILE_TOKEN)
    {
      break;
    }
    LIST_PUSH(list, token);
  }
  return list;
}
