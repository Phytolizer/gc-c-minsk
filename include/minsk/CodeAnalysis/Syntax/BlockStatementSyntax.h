#pragma once

#include "StatementSyntax.h"

struct BlockStatementSyntax
{
    struct StatementSyntax base;
    struct SyntaxToken *open_brace_token;
    struct StatementSyntaxList *statements;
    struct SyntaxToken *close_brace_token;
};

struct BlockStatementSyntax *block_statement_syntax_new(struct SyntaxToken *open_brace_token,
                                                        struct StatementSyntaxList *statements,
                                                        struct SyntaxToken *close_brace_token);
enum SyntaxKind block_statement_syntax_get_kind(struct BlockStatementSyntax *syntax);
struct SyntaxNodeList *block_statement_syntax_get_children(struct BlockStatementSyntax *syntax);
