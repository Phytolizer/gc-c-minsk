#include "minsk/CodeAnalysis/Syntax/SyntaxTree.h"

#include <minsk-private/CodeAnalysis/Syntax/Lexer.h>
#include <minsk-private/CodeAnalysis/Syntax/Parser.h>

static struct SyntaxTree* syntax_tree_new(struct SourceText* source_text)
{
    struct SyntaxTree* tree = mc_malloc(sizeof(struct SyntaxTree));
    struct Parser* parser = parser_new(source_text);
    struct CompilationUnitSyntax* root = parser_parse_compilation_unit(parser);
    struct DiagnosticBag* diagnostics = parser->diagnostics;

    tree->source_text = source_text;
    tree->diagnostics = diagnostics;
    tree->root = root;

    return tree;
}

struct SyntaxTree* syntax_tree_parse(sds text)
{
    return syntax_tree_parse_text(source_text_from(text));
}

struct SyntaxTree* syntax_tree_parse_text(struct SourceText* source_text)
{
    return syntax_tree_new(source_text);
}

struct SyntaxTokenList* syntax_tree_parse_tokens(sds text)
{
    return syntax_tree_parse_text_tokens(source_text_from(text));
}

struct SyntaxTokenList* syntax_tree_parse_text_tokens(struct SourceText* source_text)
{
    struct SyntaxTokenList* list = mc_malloc(sizeof(struct SyntaxTokenList));
    struct Lexer* lexer = lexer_new(source_text);
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
