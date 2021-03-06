#include "minsk-private/CodeAnalysis/Lowering/Lowerer.h"

static struct Lowerer* lowerer_new(void);

struct BoundStatement* lowerer_lower(struct BoundStatement* statement)
{
    struct Lowerer* lowerer = lowerer_new();
    return bound_tree_rewriter_rewrite_statement((struct BoundTreeRewriter*)lowerer, statement);
}

struct BoundExpression* lowerer_rewrite_assignment_expression(
    struct Lowerer* lowerer, struct BoundAssignmentExpression* node)
{
    struct BoundExpression* expression =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->expression);
    if (expression == node->expression)
    {
        return (struct BoundExpression*)node;
    }

    return (struct BoundExpression*)bound_assignment_expression_new(node->variable, expression);
}

struct BoundExpression* lowerer_rewrite_binary_expression(struct Lowerer* lowerer, struct BoundBinaryExpression* node)
{
    struct BoundExpression* left =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->left);
    struct BoundExpression* right =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->right);
    if (left == node->left && right == node->right)
    {
        return (struct BoundExpression*)node;
    }

    return (struct BoundExpression*)bound_binary_expression_new(left, node->op, right);
}

struct BoundStatement* lowerer_rewrite_block_statement(struct Lowerer* lowerer, struct BoundBlockStatement* node)
{
    struct BoundStatementList* statements = NULL;
    for (long i = 0; i < node->statements->length; ++i)
    {
        struct BoundStatement* old_statement = node->statements->data[i];
        struct BoundStatement* new_statement =
            bound_tree_rewriter_rewrite_statement((struct BoundTreeRewriter*)lowerer, old_statement);
        if (new_statement != old_statement)
        {
            if (statements == NULL)
            {
                statements = mc_malloc(sizeof(struct BoundStatementList));
                LIST_INIT(statements);
                LIST_RESERVE(statements, node->statements->length);
                for (long j = 0; j < i; ++j)
                {
                    LIST_PUSH(statements, node->statements->data[j]);
                }
            }
        }
        if (statements)
        {
            LIST_PUSH(statements, new_statement);
        }
    }
    if (!statements)
    {
        return (struct BoundStatement*)node;
    }

    return (struct BoundStatement*)bound_block_statement_new(statements);
}

struct BoundStatement* lowerer_rewrite_expression_statement(
    struct Lowerer* lowerer, struct BoundExpressionStatement* node)
{
    struct BoundExpression* expression =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->expression);
    if (expression == node->expression)
    {
        return (struct BoundStatement*)node;
    }

    return (struct BoundStatement*)bound_expression_statement_new(expression);
}

struct BoundStatement* lowerer_rewrite_for_statement(struct Lowerer* lowerer, struct BoundForStatement* node)
{
    // for <var> = <lower> to <upper>
    //   <body>
    //
    // --->
    //
    // {
    //   var <var> = <lower>
    //   while (<var> <= <upper>)
    //   {
    //     <body>
    //     <var> = <var> + 1
    //   }
    // }
    (void)lowerer;
    struct BoundVariableDeclaration* variable_declaration =
        bound_variable_declaration_new(node->variable, node->lower_bound);
    struct BoundVariableExpression* variable_expression = bound_variable_expression_new(node->variable);
    struct BoundBinaryExpression* condition = bound_binary_expression_new(
        (struct BoundExpression*)variable_expression,
        bind_binary_operator(SYNTAX_KIND_LESS_OR_EQUALS_TOKEN, OBJECT_KIND_INTEGER, OBJECT_KIND_INTEGER),
        node->upper_bound);
    struct BoundStatement* increment =
        (struct BoundStatement*)bound_expression_statement_new((struct BoundExpression*)bound_assignment_expression_new(
            node->variable,
            (struct BoundExpression*)bound_binary_expression_new(
                (struct BoundExpression*)variable_expression,
                bind_binary_operator(SYNTAX_KIND_PLUS_TOKEN, OBJECT_KIND_INTEGER, OBJECT_KIND_INTEGER),
                (struct BoundExpression*)bound_literal_expression_new(OBJECT_INTEGER(1)))));
    struct BoundStatementList* while_block_body = mc_malloc(sizeof(struct BoundStatementList));
    LIST_INIT(while_block_body);
    LIST_PUSH(while_block_body, node->body);
    LIST_PUSH(while_block_body, (struct BoundStatement*)increment);
    struct BoundBlockStatement* while_block = bound_block_statement_new(while_block_body);
    struct BoundWhileStatement* while_statement =
        bound_while_statement_new((struct BoundExpression*)condition, (struct BoundStatement*)while_block);
    struct BoundStatementList* result_body = mc_malloc(sizeof(struct BoundStatementList));
    LIST_INIT(result_body);
    LIST_PUSH(result_body, (struct BoundStatement*)variable_declaration);
    LIST_PUSH(result_body, (struct BoundStatement*)while_statement);
    struct BoundBlockStatement* result = bound_block_statement_new(result_body);
    return (struct BoundStatement*)result;
}

struct BoundStatement* lowerer_rewrite_if_statement(struct Lowerer* lowerer, struct BoundIfStatement* node)
{
    struct BoundExpression* condition =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->condition);
    struct BoundStatement* then_statement =
        bound_tree_rewriter_rewrite_statement((struct BoundTreeRewriter*)lowerer, node->then_statement);
    struct BoundStatement* else_statement = node->else_statement != NULL
        ? bound_tree_rewriter_rewrite_statement((struct BoundTreeRewriter*)lowerer, node->else_statement)
        : NULL;
    if (condition == node->condition && then_statement == node->then_statement &&
        else_statement == node->else_statement)
    {
        return (struct BoundStatement*)node;
    }
    return (struct BoundStatement*)bound_if_statement_new(condition, then_statement, else_statement);
}

struct BoundExpression* lowerer_rewrite_literal_expression(struct Lowerer* lowerer, struct BoundLiteralExpression* node)
{
    (void)lowerer;
    return (struct BoundExpression*)node;
}

struct BoundExpression* lowerer_rewrite_unary_expression(struct Lowerer* lowerer, struct BoundUnaryExpression* node)
{
    struct BoundExpression* operand =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->operand);
    if (operand == node->operand)
    {
        return (struct BoundExpression*)node;
    }

    return (struct BoundExpression*)bound_unary_expression_new(node->op, operand);
}

struct BoundStatement* lowerer_rewrite_variable_declaration(
    struct Lowerer* lowerer, struct BoundVariableDeclaration* node)
{
    struct BoundExpression* initializer =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->initializer);
    if (initializer == node->initializer)
    {
        return (struct BoundStatement*)node;
    }
    return (struct BoundStatement*)bound_variable_declaration_new(node->variable, initializer);
}

struct BoundExpression* lowerer_rewrite_variable_expression(
    struct Lowerer* lowerer, struct BoundVariableExpression* node)
{
    (void)lowerer;
    return (struct BoundExpression*)node;
}

struct BoundStatement* lowerer_rewrite_while_statement(struct Lowerer* lowerer, struct BoundWhileStatement* node)
{
    struct BoundExpression* condition =
        bound_tree_rewriter_rewrite_expression((struct BoundTreeRewriter*)lowerer, node->condition);
    struct BoundStatement* body = bound_tree_rewriter_rewrite_statement((struct BoundTreeRewriter*)lowerer, node->body);
    if (condition == node->condition && body == node->body)
    {
        return (struct BoundStatement*)node;
    }

    return (struct BoundStatement*)bound_while_statement_new(node->condition, node->body);
}

static struct Lowerer* lowerer_new(void)
{
    struct Lowerer* lowerer = mc_malloc(sizeof(struct Lowerer));
    lowerer->base.kind = BOUND_TREE_REWRITER_KIND_LOWERER;
    return lowerer;
}
