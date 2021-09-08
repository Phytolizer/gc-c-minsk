#pragma once

#include <common/List.h>

#include "SyntaxNode.h"

#define STATEMENT_SYNTAX_KINDS_                                                                                        \
    X(BLOCK_STATEMENT_SYNTAX)                                                                                          \
    X(EXPRESSION_STATEMENT_SYNTAX)                                                                                     \
    X(IF_STATEMENT_SYNTAX)                                                                                             \
    X(VARIABLE_DECLARATION_SYNTAX)                                                                                     \
    X(WHILE_STATEMENT_SYNTAX)

enum StatementSyntaxKind
{
#define X(x) STATEMENT_SYNTAX_KIND_##x,
    STATEMENT_SYNTAX_KINDS_
#undef X
};

struct StatementSyntax
{
    struct SyntaxNode base;
    enum StatementSyntaxKind kind;
};

DECLARE_NAMED_LIST(StatementSyntaxList, struct StatementSyntax *);

void statement_syntax_init(struct StatementSyntax *syntax, enum StatementSyntaxKind kind);
enum SyntaxKind statement_syntax_get_kind(struct StatementSyntax *syntax);
struct SyntaxNodeList *statement_syntax_get_children(struct StatementSyntax *syntax);
