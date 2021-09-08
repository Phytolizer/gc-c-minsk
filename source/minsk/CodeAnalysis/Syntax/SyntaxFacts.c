#include "minsk/CodeAnalysis/Syntax/SyntaxFacts.h"

#include <string.h>

int unary_operator_precedence(enum SyntaxKind kind)
{
    switch (kind)
    {
    case SYNTAX_KIND_PLUS_TOKEN:
    case SYNTAX_KIND_MINUS_TOKEN:
    case SYNTAX_KIND_BANG_TOKEN:
        return 6;
    default:
        return 0;
    }
}

int binary_operator_precedence(enum SyntaxKind kind)
{
    switch (kind)
    {
    case SYNTAX_KIND_STAR_TOKEN:
    case SYNTAX_KIND_SLASH_TOKEN:
        return 5;
    case SYNTAX_KIND_PLUS_TOKEN:
    case SYNTAX_KIND_MINUS_TOKEN:
        return 4;
    case SYNTAX_KIND_EQUALS_EQUALS_TOKEN:
    case SYNTAX_KIND_BANG_EQUALS_TOKEN:
    case SYNTAX_KIND_LESS_TOKEN:
    case SYNTAX_KIND_LESS_OR_EQUALS_TOKEN:
    case SYNTAX_KIND_GREATER_TOKEN:
    case SYNTAX_KIND_GREATER_OR_EQUALS_TOKEN:
        return 3;
    case SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN:
        return 2;
    case SYNTAX_KIND_PIPE_PIPE_TOKEN:
        return 1;
    default:
        return 0;
    }
}

enum SyntaxKind keyword_kind(sds text)
{
    if (strcmp(text, "else") == 0)
    {
        return SYNTAX_KIND_ELSE_KEYWORD;
    }
    if (strcmp(text, "false") == 0)
    {
        return SYNTAX_KIND_FALSE_KEYWORD;
    }
    if (strcmp(text, "if") == 0)
    {
        return SYNTAX_KIND_IF_KEYWORD;
    }
    if (strcmp(text, "let") == 0)
    {
        return SYNTAX_KIND_LET_KEYWORD;
    }
    if (strcmp(text, "true") == 0)
    {
        return SYNTAX_KIND_TRUE_KEYWORD;
    }
    if (strcmp(text, "var") == 0)
    {
        return SYNTAX_KIND_VAR_KEYWORD;
    }
    if (strcmp(text, "while") == 0)
    {
        return SYNTAX_KIND_WHILE_KEYWORD;
    }
    return SYNTAX_KIND_IDENTIFIER_TOKEN;
}

sds syntax_facts_get_text(enum SyntaxKind kind)
{
    switch (kind)
    {
    case SYNTAX_KIND_PLUS_TOKEN:
        return sdsnew("+");
    case SYNTAX_KIND_MINUS_TOKEN:
        return sdsnew("-");
    case SYNTAX_KIND_STAR_TOKEN:
        return sdsnew("*");
    case SYNTAX_KIND_SLASH_TOKEN:
        return sdsnew("/");
    case SYNTAX_KIND_BANG_TOKEN:
        return sdsnew("!");
    case SYNTAX_KIND_EQUALS_TOKEN:
        return sdsnew("=");
    case SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN:
        return sdsnew("&&");
    case SYNTAX_KIND_PIPE_PIPE_TOKEN:
        return sdsnew("||");
    case SYNTAX_KIND_EQUALS_EQUALS_TOKEN:
        return sdsnew("==");
    case SYNTAX_KIND_BANG_EQUALS_TOKEN:
        return sdsnew("!=");
    case SYNTAX_KIND_LESS_TOKEN:
        return sdsnew("<");
    case SYNTAX_KIND_LESS_OR_EQUALS_TOKEN:
        return sdsnew("<=");
    case SYNTAX_KIND_GREATER_TOKEN:
        return sdsnew(">");
    case SYNTAX_KIND_GREATER_OR_EQUALS_TOKEN:
        return sdsnew(">=");
    case SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN:
        return sdsnew("(");
    case SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN:
        return sdsnew(")");
    case SYNTAX_KIND_OPEN_BRACE_TOKEN:
        return sdsnew("{");
    case SYNTAX_KIND_CLOSE_BRACE_TOKEN:
        return sdsnew("}");
    case SYNTAX_KIND_ELSE_KEYWORD:
        return sdsnew("else");
    case SYNTAX_KIND_FALSE_KEYWORD:
        return sdsnew("false");
    case SYNTAX_KIND_IF_KEYWORD:
        return sdsnew("if");
    case SYNTAX_KIND_LET_KEYWORD:
        return sdsnew("let");
    case SYNTAX_KIND_TRUE_KEYWORD:
        return sdsnew("true");
    case SYNTAX_KIND_VAR_KEYWORD:
        return sdsnew("var");
    case SYNTAX_KIND_WHILE_KEYWORD:
        return sdsnew("while");
    default:
        return NULL;
    }
}
