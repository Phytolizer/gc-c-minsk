#include "minsk-private/CodeAnalysis/Binding/BoundTreeRewriter.h"

#include <assert.h>
#include <minsk-private/CodeAnalysis/Binding/BoundAssignmentExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundBinaryExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundBlockStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundExpressionStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundForStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundIfStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundLiteralExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundUnaryExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundVariableDeclaration.h>
#include <minsk-private/CodeAnalysis/Binding/BoundVariableExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundWhileStatement.h>
#include <minsk-private/CodeAnalysis/Lowering/Lowerer.h>

#define DEFINE_DISPATCH(ReturnType, ArgumentType, FunctionName) \
    static struct ReturnType* FunctionName(struct BoundTreeRewriter* rewriter, struct ArgumentType* node) \
    { \
        switch (rewriter->kind) \
        { \
        case BOUND_TREE_REWRITER_KIND_LOWERER: \
            return lowerer_##FunctionName((struct Lowerer*)rewriter, node); \
        } \
        assert(false && "Corrupt bound tree rewriter kind"); \
    }

DEFINE_DISPATCH(BoundExpression, BoundAssignmentExpression, rewrite_assignment_expression)
DEFINE_DISPATCH(BoundExpression, BoundBinaryExpression, rewrite_binary_expression)
DEFINE_DISPATCH(BoundStatement, BoundBlockStatement, rewrite_block_statement)
DEFINE_DISPATCH(BoundStatement, BoundExpressionStatement, rewrite_expression_statement)
DEFINE_DISPATCH(BoundStatement, BoundForStatement, rewrite_for_statement)
DEFINE_DISPATCH(BoundStatement, BoundIfStatement, rewrite_if_statement)
DEFINE_DISPATCH(BoundExpression, BoundLiteralExpression, rewrite_literal_expression)
DEFINE_DISPATCH(BoundExpression, BoundUnaryExpression, rewrite_unary_expression)
DEFINE_DISPATCH(BoundStatement, BoundVariableDeclaration, rewrite_variable_declaration)
DEFINE_DISPATCH(BoundExpression, BoundVariableExpression, rewrite_variable_expression)
DEFINE_DISPATCH(BoundStatement, BoundWhileStatement, rewrite_while_statement)

struct BoundStatement* bound_tree_rewriter_rewrite_statement(
    struct BoundTreeRewriter* rewriter, struct BoundStatement* node)
{
    switch (node->kind)
    {
    case BOUND_STATEMENT_KIND_BLOCK:
        return rewrite_block_statement(rewriter, (struct BoundBlockStatement*)node);
    case BOUND_STATEMENT_KIND_EXPRESSION:
        return rewrite_expression_statement(rewriter, (struct BoundExpressionStatement*)node);
    case BOUND_STATEMENT_KIND_FOR:
        return rewrite_for_statement(rewriter, (struct BoundForStatement*)node);
    case BOUND_STATEMENT_KIND_IF:
        return rewrite_if_statement(rewriter, (struct BoundIfStatement*)node);
    case BOUND_STATEMENT_KIND_VARIABLE_DECLARATION:
        return rewrite_variable_declaration(rewriter, (struct BoundVariableDeclaration*)node);
    case BOUND_STATEMENT_KIND_WHILE:
        return rewrite_while_statement(rewriter, (struct BoundWhileStatement*)node);
    }
    assert(false && "Corrupt node kind");
}

struct BoundExpression* bound_tree_rewriter_rewrite_expression(
    struct BoundTreeRewriter* rewriter, struct BoundExpression* node)
{
    switch (node->kind)
    {
    case BOUND_EXPRESSION_KIND_BOUND_BINARY_EXPRESSION:
        return rewrite_binary_expression(rewriter, (struct BoundBinaryExpression*)node);
    case BOUND_EXPRESSION_KIND_BOUND_LITERAL_EXPRESSION:
        return rewrite_literal_expression(rewriter, (struct BoundLiteralExpression*)node);
    case BOUND_EXPRESSION_KIND_BOUND_UNARY_EXPRESSION:
        return rewrite_unary_expression(rewriter, (struct BoundUnaryExpression*)node);
    case BOUND_EXPRESSION_KIND_BOUND_VARIABLE_EXPRESSION:
        return rewrite_variable_expression(rewriter, (struct BoundVariableExpression*)node);
    case BOUND_EXPRESSION_KIND_BOUND_ASSIGNMENT_EXPRESSION:
        return rewrite_assignment_expression(rewriter, (struct BoundAssignmentExpression*)node);
    }
    assert(false && "Corrupt node kind");
}
