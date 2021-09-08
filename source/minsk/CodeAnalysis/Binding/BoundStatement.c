#include "BoundStatement.h"

#include "BoundBlockStatement.h"
#include "BoundExpressionStatement.h"
#include "BoundForStatement.h"
#include "BoundIfStatement.h"
#include "BoundVariableDeclaration.h"
#include "BoundWhileStatement.h"

void bound_statement_init(struct BoundStatement* stmt, enum BoundStatementKind kind)
{
    stmt->base.type = BOUND_NODE_TYPE_STATEMENT;
    stmt->kind = kind;
}

enum BoundNodeKind bound_statement_get_kind(struct BoundStatement* stmt)
{
    switch (stmt->kind)
    {
    case BOUND_STATEMENT_KIND_BLOCK:
        return bound_block_statement_get_kind((struct BoundBlockStatement*)stmt);
    case BOUND_STATEMENT_KIND_EXPRESSION:
        return bound_expression_statement_get_kind((struct BoundExpressionStatement*)stmt);
    case BOUND_STATEMENT_KIND_FOR:
        return bound_for_statement_get_kind((struct BoundForStatement*)stmt);
    case BOUND_STATEMENT_KIND_IF:
        return bound_if_statement_get_kind((struct BoundIfStatement*)stmt);
    case BOUND_STATEMENT_KIND_VARIABLE_DECLARATION:
        return bound_variable_declaration_get_kind((struct BoundVariableDeclaration*)stmt);
    case BOUND_STATEMENT_KIND_WHILE:
        return bound_while_statement_get_kind((struct BoundWhileStatement*)stmt);
    }
}
