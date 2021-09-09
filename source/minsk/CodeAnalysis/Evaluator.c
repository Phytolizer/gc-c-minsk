#include "minsk-private/CodeAnalysis/Evaluator.h"

#include <assert.h>
#include <stdio.h>

#include <minsk-private/CodeAnalysis/Binding/BoundAssignmentExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundBinaryExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundBinaryOperatorKind.h>
#include <minsk-private/CodeAnalysis/Binding/BoundBlockStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundExpressionStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundForStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundIfStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundLiteralExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundUnaryExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundUnaryOperatorKind.h>
#include <minsk-private/CodeAnalysis/Binding/BoundVariableDeclaration.h>
#include <minsk-private/CodeAnalysis/Binding/BoundVariableExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundWhileStatement.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/VariableStore.h>

static void evaluate_statement(struct Evaluator* evaluator, struct BoundStatement* statement);

static void evaluate_block_statement(struct Evaluator* evaluator, struct BoundBlockStatement* stmt);
static void evaluate_expression_statement(struct Evaluator* evaluator, struct BoundExpressionStatement* stmt);
static void evaluate_for_statement(struct Evaluator* evaluator, struct BoundForStatement* stmt);
static void evaluate_if_statement(struct Evaluator* evaluator, struct BoundIfStatement* stmt);
static void evaluate_variable_declaration(struct Evaluator* evaluator, struct BoundVariableDeclaration* stmt);
static void evaluate_while_statement(struct Evaluator* evaluator, struct BoundWhileStatement* stmt);

static struct Object* evaluate_expression(struct Evaluator* evaluator, struct BoundExpression* root);

static struct Object* evaluate_literal_expression(struct Evaluator* evaluator, struct BoundLiteralExpression* expr);
static struct Object* evaluate_unary_expression(struct Evaluator* evaluator, struct BoundUnaryExpression* expr);
static struct Object* evaluate_binary_expression(struct Evaluator* evaluator, struct BoundBinaryExpression* expr);
static struct Object* evaluate_variable_expression(struct Evaluator* evaluator, struct BoundVariableExpression* expr);
static struct Object* evaluate_assignment_expression(
    struct Evaluator* evaluator, struct BoundAssignmentExpression* expr);

struct Evaluator* evaluator_new(struct BoundStatement* root, struct VariableStore* variables)
{
    struct Evaluator* evaluator = mc_malloc(sizeof(struct Evaluator));
    evaluator->root = root;
    evaluator->variables = variables;
    evaluator->last_value = OBJECT_NULL();
    return evaluator;
}

struct Object* evaluator_evaluate(struct Evaluator* evaluator)
{
    evaluate_statement(evaluator, evaluator->root);
    return evaluator->last_value;
}

static void evaluate_statement(struct Evaluator* evaluator, struct BoundStatement* stmt)
{
    switch (bound_statement_get_kind(stmt))
    {
    case BOUND_NODE_KIND_BLOCK_STATEMENT:
        evaluate_block_statement(evaluator, (struct BoundBlockStatement*)stmt);
        break;
    case BOUND_NODE_KIND_EXPRESSION_STATEMENT:
        evaluate_expression_statement(evaluator, (struct BoundExpressionStatement*)stmt);
        break;
    case BOUND_NODE_KIND_FOR_STATEMENT:
        evaluate_for_statement(evaluator, (struct BoundForStatement*)stmt);
        break;
    case BOUND_NODE_KIND_IF_STATEMENT:
        evaluate_if_statement(evaluator, (struct BoundIfStatement*)stmt);
        break;
    case BOUND_NODE_KIND_VARIABLE_DECLARATION:
        evaluate_variable_declaration(evaluator, (struct BoundVariableDeclaration*)stmt);
        break;
    case BOUND_NODE_KIND_WHILE_STATEMENT:
        evaluate_while_statement(evaluator, (struct BoundWhileStatement*)stmt);
        break;
    default:
        fprintf(stderr, "unhandled syntax node %s\n", BOUND_NODE_KINDS[bound_statement_get_kind(stmt)]);
        assert(false && "unhandled syntax node");
    }
}

static void evaluate_block_statement(struct Evaluator* evaluator, struct BoundBlockStatement* stmt)
{
    for (long i = 0; i < stmt->statements->length; ++i)
    {
        evaluate_statement(evaluator, stmt->statements->data[i]);
    }
}

static void evaluate_expression_statement(struct Evaluator* evaluator, struct BoundExpressionStatement* stmt)
{
    evaluator->last_value = evaluate_expression(evaluator, stmt->expression);
}

static void evaluate_for_statement(struct Evaluator* evaluator, struct BoundForStatement* stmt)
{
    int lower_bound = OBJECT_AS_INTEGER(evaluate_expression(evaluator, stmt->lower_bound))->value;
    int upper_bound = OBJECT_AS_INTEGER(evaluate_expression(evaluator, stmt->upper_bound))->value;

    for (int i = lower_bound; i <= upper_bound; ++i)
    {
        variable_store_insert_or_assign(evaluator->variables, stmt->variable, OBJECT_INTEGER(i));
        evaluate_statement(evaluator, stmt->body);
    }
}

static void evaluate_if_statement(struct Evaluator* evaluator, struct BoundIfStatement* stmt)
{
    struct Object* condition = evaluate_expression(evaluator, stmt->condition);
    if (OBJECT_AS_BOOLEAN(condition)->value)
    {
        evaluate_statement(evaluator, stmt->then_statement);
    }
    else if (stmt->else_statement)
    {
        evaluate_statement(evaluator, stmt->else_statement);
    }
}

static void evaluate_variable_declaration(struct Evaluator* evaluator, struct BoundVariableDeclaration* stmt)
{
    struct Object* value = evaluate_expression(evaluator, stmt->initializer);
    variable_store_insert_or_assign(evaluator->variables, stmt->variable, value);
    evaluator->last_value = value;
}

static void evaluate_while_statement(struct Evaluator* evaluator, struct BoundWhileStatement* stmt)
{
    while (OBJECT_AS_BOOLEAN(evaluate_expression(evaluator, stmt->condition))->value)
    {
        evaluate_statement(evaluator, stmt->body);
    }
}

static struct Object* evaluate_expression(struct Evaluator* evaluator, struct BoundExpression* root)
{
    switch (bound_expression_get_kind(root))
    {
    case BOUND_NODE_KIND_LITERAL_EXPRESSION:
        return evaluate_literal_expression(evaluator, (struct BoundLiteralExpression*)root);
    case BOUND_NODE_KIND_UNARY_EXPRESSION:
        return evaluate_unary_expression(evaluator, (struct BoundUnaryExpression*)root);
    case BOUND_NODE_KIND_BINARY_EXPRESSION:
        return evaluate_binary_expression(evaluator, (struct BoundBinaryExpression*)root);
    case BOUND_EXPRESSION_KIND_BOUND_VARIABLE_EXPRESSION:
        return evaluate_variable_expression(evaluator, (struct BoundVariableExpression*)root);
    case BOUND_EXPRESSION_KIND_BOUND_ASSIGNMENT_EXPRESSION:
        return evaluate_assignment_expression(evaluator, (struct BoundAssignmentExpression*)root);
    default:
        fprintf(stderr, "unhandled syntax node %s\n", BOUND_NODE_KINDS[bound_expression_get_kind(root)]);
        assert(false && "unhandled syntax node");
    }
}

static struct Object* evaluate_literal_expression(struct Evaluator* evaluator, struct BoundLiteralExpression* expr)
{
    (void)evaluator;
    return expr->value;
}

static struct Object* evaluate_unary_expression(struct Evaluator* evaluator, struct BoundUnaryExpression* expr)
{
    struct Object* operand = evaluate_expression(evaluator, expr->operand);
    switch (expr->op->kind)
    {
    case BOUND_UNARY_OPERATOR_KIND_IDENTITY:
        return operand;
    case BOUND_UNARY_OPERATOR_KIND_NEGATION:
        return OBJECT_INTEGER(-OBJECT_AS_INTEGER(operand)->value);
    case BOUND_UNARY_OPERATOR_KIND_LOGICAL_NEGATION:
        return OBJECT_BOOLEAN(!OBJECT_AS_BOOLEAN(operand)->value);
    case BOUND_UNARY_OPERATOR_KIND_ONES_COMPLEMENT:
        return OBJECT_INTEGER(~OBJECT_AS_INTEGER(operand)->value);
    }
}

static struct Object* evaluate_binary_expression(struct Evaluator* evaluator, struct BoundBinaryExpression* expr)
{
    struct Object* left = evaluate_expression(evaluator, expr->left);
    struct Object* right = evaluate_expression(evaluator, expr->right);

    switch (expr->op->kind)
    {
    case BOUND_BINARY_OPERATOR_KIND_ADDITION:
        return OBJECT_INTEGER(OBJECT_AS_INTEGER(left)->value + OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_SUBTRACTION:
        return OBJECT_INTEGER(OBJECT_AS_INTEGER(left)->value - OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION:
        return OBJECT_INTEGER(OBJECT_AS_INTEGER(left)->value * OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_DIVISION:
        return OBJECT_INTEGER(OBJECT_AS_INTEGER(left)->value / OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_LOGICAL_AND:
        return OBJECT_BOOLEAN(OBJECT_AS_BOOLEAN(left)->value && OBJECT_AS_BOOLEAN(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_LOGICAL_OR:
        return OBJECT_BOOLEAN(OBJECT_AS_BOOLEAN(left)->value || OBJECT_AS_BOOLEAN(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_EQUALITY:
        return OBJECT_BOOLEAN(objects_equal(left, right));
    case BOUND_BINARY_OPERATOR_KIND_INEQUALITY:
        return OBJECT_BOOLEAN(!objects_equal(left, right));
    case BOUND_BINARY_OPERATOR_KIND_LESS:
        return OBJECT_BOOLEAN(OBJECT_AS_INTEGER(left)->value < OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_LESS_OR_EQUALS:
        return OBJECT_BOOLEAN(OBJECT_AS_INTEGER(left)->value <= OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_GREATER:
        return OBJECT_BOOLEAN(OBJECT_AS_INTEGER(left)->value > OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_GREATER_OR_EQUALS:
        return OBJECT_BOOLEAN(OBJECT_AS_INTEGER(left)->value >= OBJECT_AS_INTEGER(right)->value);
    case BOUND_BINARY_OPERATOR_KIND_BITWISE_AND:
        if (right->kind == OBJECT_KIND_INTEGER)
        {
            return OBJECT_INTEGER(OBJECT_AS_INTEGER(left)->value & OBJECT_AS_INTEGER(right)->value);
        }
        else
        {
            return OBJECT_BOOLEAN(OBJECT_AS_BOOLEAN(left)->value && OBJECT_AS_BOOLEAN(right)->value);
        }
    case BOUND_BINARY_OPERATOR_KIND_BITWISE_OR:
        if (right->kind == OBJECT_KIND_INTEGER)
        {
            return OBJECT_INTEGER(OBJECT_AS_INTEGER(left)->value | OBJECT_AS_INTEGER(right)->value);
        }
        else
        {
            return OBJECT_BOOLEAN(OBJECT_AS_BOOLEAN(left)->value || OBJECT_AS_BOOLEAN(right)->value);
        }
    case BOUND_BINARY_OPERATOR_KIND_BITWISE_XOR:
        if (right->kind == OBJECT_KIND_INTEGER)
        {
            return OBJECT_INTEGER(OBJECT_AS_INTEGER(left)->value ^ OBJECT_AS_INTEGER(right)->value);
        }
        else
        {
            return OBJECT_BOOLEAN(OBJECT_AS_BOOLEAN(left)->value ^ OBJECT_AS_BOOLEAN(right)->value);
        }
    }
}

static struct Object* evaluate_variable_expression(struct Evaluator* evaluator, struct BoundVariableExpression* expr)
{
    return *variable_store_lookup(evaluator->variables, expr->variable);
}

static struct Object* evaluate_assignment_expression(
    struct Evaluator* evaluator, struct BoundAssignmentExpression* expr)
{
    struct Object* value = evaluate_expression(evaluator, expr->expression);
    variable_store_insert_or_assign(evaluator->variables, expr->variable, value);
    return value;
}
