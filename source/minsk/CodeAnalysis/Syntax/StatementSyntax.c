#include "minsk/CodeAnalysis/Syntax/StatementSyntax.h"

#include <minsk/CodeAnalysis/Syntax/BlockStatementSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ExpressionStatementSyntax.h>

void statement_syntax_init(
    struct StatementSyntax* syntax,
    enum StatementSyntaxKind kind)
{
  syntax->base.kind = SYNTAX_NODE_KIND_STATEMENT;
  syntax->kind = kind;
}

enum SyntaxKind statement_syntax_get_kind(struct StatementSyntax* syntax)
{
  switch (syntax->kind)
  {
    case STATEMENT_SYNTAX_KIND_BLOCK_STATEMENT_SYNTAX:
      return block_statement_syntax_get_kind(
          (struct BlockStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_EXPRESSION_STATEMENT_SYNTAX:
      return expression_statement_syntax_get_kind(
          (struct ExpressionStatementSyntax*)syntax);
  }
}

struct SyntaxNodeList* statement_syntax_get_children(
    struct StatementSyntax* syntax)
{
  switch (syntax->kind)
  {
    case STATEMENT_SYNTAX_KIND_BLOCK_STATEMENT_SYNTAX:
      return block_statement_syntax_get_children(
          (struct BlockStatementSyntax*)syntax);
    case STATEMENT_SYNTAX_KIND_EXPRESSION_STATEMENT_SYNTAX:
      return expression_statement_syntax_get_children(
          (struct ExpressionStatementSyntax*)syntax);
  }
}
