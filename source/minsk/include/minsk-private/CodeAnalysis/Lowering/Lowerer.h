#pragma once

#include <minsk-private/CodeAnalysis/Binding/BoundAssignmentExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundBinaryExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundBlockStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundExpressionStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundForStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundIfStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundLiteralExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundTreeRewriter.h>
#include <minsk-private/CodeAnalysis/Binding/BoundUnaryExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundVariableDeclaration.h>
#include <minsk-private/CodeAnalysis/Binding/BoundVariableExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundWhileStatement.h>

struct Lowerer
{
    struct BoundTreeRewriter base;
};

struct BoundStatement* lowerer_lower(struct BoundStatement* statement);
struct BoundExpression* lowerer_rewrite_assignment_expression(
    struct Lowerer* lowerer, struct BoundAssignmentExpression* expression);
struct BoundExpression* lowerer_rewrite_binary_expression(
    struct Lowerer* lowerer, struct BoundBinaryExpression* expression);
struct BoundStatement* lowerer_rewrite_block_statement(struct Lowerer* lowerer, struct BoundBlockStatement* statement);
struct BoundStatement* lowerer_rewrite_expression_statement(
    struct Lowerer* lowerer, struct BoundExpressionStatement* statement);
struct BoundStatement* lowerer_rewrite_for_statement(struct Lowerer* lowerer, struct BoundForStatement* statement);
struct BoundStatement* lowerer_rewrite_if_statement(struct Lowerer* lowerer, struct BoundIfStatement* statement);
struct BoundExpression* lowerer_rewrite_literal_expression(
    struct Lowerer* lowerer, struct BoundLiteralExpression* expression);
struct BoundExpression* lowerer_rewrite_unary_expression(
    struct Lowerer* lowerer, struct BoundUnaryExpression* expression);
struct BoundStatement* lowerer_rewrite_variable_declaration(
    struct Lowerer* lowerer, struct BoundVariableDeclaration* statement);
struct BoundExpression* lowerer_rewrite_variable_expression(
    struct Lowerer* lowerer, struct BoundVariableExpression* expression);
struct BoundStatement* lowerer_rewrite_while_statement(struct Lowerer* lowerer, struct BoundWhileStatement* statement);
