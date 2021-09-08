#include "minsk/CodeAnalysis/Syntax/StatementSyntax.h"
#include "minsk/CodeAnalysis/Syntax/ForStatementSyntax.h"

#include <minsk/CodeAnalysis/Syntax/BlockStatementSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ExpressionStatementSyntax.h>
#include <minsk/CodeAnalysis/Syntax/IfStatementSyntax.h>
#include <minsk/CodeAnalysis/Syntax/VariableDeclarationSyntax.h>
#include <minsk/CodeAnalysis/Syntax/WhileStatementSyntax.h>

void statement_syntax_init(struct StatementSyntax* syntax, enum StatementSyntaxKind kind)
{
    syntax->base.kind = SYNTAX_NODE_KIND_STATEMENT;
    syntax->kind = kind;
}

enum SyntaxKind statement_syntax_get_kind(struct StatementSyntax* syntax)
{
    switch (syntax->kind)
    {
    case STATEMENT_SYNTAX_KIND_BLOCK_STATEMENT_SYNTAX:
        return block_statement_syntax_get_kind((struct BlockStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_EXPRESSION_STATEMENT_SYNTAX:
        return expression_statement_syntax_get_kind((struct ExpressionStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_FOR_STATEMENT_SYNTAX:
        return for_statement_syntax_get_kind((struct ForStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_IF_STATEMENT_SYNTAX:
        return if_statement_syntax_get_kind((struct IfStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_VARIABLE_DECLARATION_SYNTAX:
        return variable_declaration_syntax_get_kind((struct VariableDeclarationSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_WHILE_STATEMENT_SYNTAX:
        return while_statement_syntax_get_kind((struct WhileStatementSyntax*)syntax);
    }
}

struct SyntaxNodeList* statement_syntax_get_children(struct StatementSyntax* syntax)
{
    switch (syntax->kind)
    {
    case STATEMENT_SYNTAX_KIND_BLOCK_STATEMENT_SYNTAX:
        return block_statement_syntax_get_children((struct BlockStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_EXPRESSION_STATEMENT_SYNTAX:
        return expression_statement_syntax_get_children((struct ExpressionStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_FOR_STATEMENT_SYNTAX:
        return for_statement_syntax_get_children((struct ForStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_IF_STATEMENT_SYNTAX:
        return if_statement_syntax_get_children((struct IfStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_VARIABLE_DECLARATION_SYNTAX:
        return variable_declaration_syntax_get_children((struct VariableDeclarationSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_WHILE_STATEMENT_SYNTAX:
        return while_statement_syntax_get_children((struct WhileStatementSyntax*)syntax);
    }
}
