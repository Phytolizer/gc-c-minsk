#pragma once

#include <stddef.h>

#define SYNTAX_KINDS_                                                                                                  \
    X(NUMBER_TOKEN)                                                                                                    \
    X(WHITESPACE_TOKEN)                                                                                                \
    X(PLUS_TOKEN)                                                                                                      \
    X(MINUS_TOKEN)                                                                                                     \
    X(STAR_TOKEN)                                                                                                      \
    X(SLASH_TOKEN)                                                                                                     \
    X(OPEN_PARENTHESIS_TOKEN)                                                                                          \
    X(CLOSE_PARENTHESIS_TOKEN)                                                                                         \
    X(OPEN_BRACE_TOKEN)                                                                                                \
    X(CLOSE_BRACE_TOKEN)                                                                                               \
    X(BANG_TOKEN)                                                                                                      \
    X(AMPERSAND_AMPERSAND_TOKEN)                                                                                       \
    X(PIPE_PIPE_TOKEN)                                                                                                 \
    X(EQUALS_EQUALS_TOKEN)                                                                                             \
    X(BANG_EQUALS_TOKEN)                                                                                               \
    X(LESS_OR_EQUALS_TOKEN)                                                                                            \
    X(LESS_TOKEN)                                                                                                      \
    X(GREATER_OR_EQUALS_TOKEN)                                                                                         \
    X(GREATER_TOKEN)                                                                                                   \
    X(EQUALS_TOKEN)                                                                                                    \
    X(IDENTIFIER_TOKEN)                                                                                                \
    X(BAD_TOKEN)                                                                                                       \
    X(END_OF_FILE_TOKEN)                                                                                               \
    X(ELSE_KEYWORD)                                                                                                    \
    X(FALSE_KEYWORD)                                                                                                   \
    X(IF_KEYWORD)                                                                                                      \
    X(LET_KEYWORD)                                                                                                     \
    X(TRUE_KEYWORD)                                                                                                    \
    X(VAR_KEYWORD)                                                                                                     \
    X(WHILE_KEYWORD)                                                                                                   \
    X(LITERAL_EXPRESSION)                                                                                              \
    X(BINARY_EXPRESSION)                                                                                               \
    X(PARENTHESIZED_EXPRESSION)                                                                                        \
    X(UNARY_EXPRESSION)                                                                                                \
    X(NAME_EXPRESSION)                                                                                                 \
    X(ASSIGNMENT_EXPRESSION)                                                                                           \
    X(COMPILATION_UNIT)                                                                                                \
    X(BLOCK_STATEMENT)                                                                                                 \
    X(EXPRESSION_STATEMENT)                                                                                            \
    X(IF_STATEMENT)                                                                                                    \
    X(VARIABLE_DECLARATION)                                                                                            \
    X(WHILE_STATEMENT)                                                                                                 \
    X(ELSE_CLAUSE)

extern const char *const SYNTAX_KINDS[];

enum SyntaxKind
{
#define X(x) SYNTAX_KIND_##x,
    SYNTAX_KINDS_
#undef X
};

extern const enum SyntaxKind SYNTAX_KIND_VARIANTS[];
extern const size_t NUM_SYNTAX_KIND_VARIANTS;
