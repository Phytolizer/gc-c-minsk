#include "Binder.h"

#include <assert.h>
#include <stdio.h>

#include <IncludeMe.h>
#include <common/Nullable.h>
#include <common/Object.h>
#include <minsk/CodeAnalysis/Syntax/AssignmentExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/NameExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>

#include "BoundAssignmentExpression.h"
#include "BoundBinaryExpression.h"
#include "BoundBinaryOperator.h"
#include "BoundExpression.h"
#include "BoundLiteralExpression.h"
#include "BoundUnaryExpression.h"
#include "BoundUnaryOperator.h"
#include "BoundVariableExpression.h"

static struct BoundExpression* bind_expression(
    struct Binder* binder,
    struct ExpressionSyntax* syntax);
static struct BoundExpression* bind_literal_expression(
    struct Binder* binder,
    struct LiteralExpressionSyntax* syntax);
static struct BoundExpression* bind_binary_expression(
    struct Binder* binder,
    struct BinaryExpressionSyntax* syntax);
static struct BoundExpression* bind_parenthesized_expression(
    struct Binder* binder,
    struct ParenthesizedExpressionSyntax* syntax);
static struct BoundExpression* bind_unary_expression(
    struct Binder* binder,
    struct UnaryExpressionSyntax* syntax);
static struct BoundExpression* bind_name_expression(
    struct Binder* binder,
    struct NameExpressionSyntax* syntax);
static struct BoundExpression* bind_assignment_expression(
    struct Binder* binder,
    struct AssignmentExpressionSyntax* syntax);

struct Binder* binder_new(struct VariableStore* variables)
{
  struct Binder* binder = mc_malloc(sizeof(struct Binder));
  binder->diagnostics = diagnostic_bag_new();
  binder->variables = variables;
  return binder;
}

struct BoundExpression* binder_bind(
    struct Binder* binder,
    struct ExpressionSyntax* syntax)
{
  return bind_expression(binder, syntax);
}

static struct BoundExpression* bind_expression(
    struct Binder* binder,
    struct ExpressionSyntax* syntax)
{
  switch (syntax->kind)
  {
    case EXPRESSION_SYNTAX_KIND_LITERAL_EXPRESSION_SYNTAX:
      return bind_literal_expression(
          binder,
          (struct LiteralExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX:
      return bind_binary_expression(
          binder,
          (struct BinaryExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_PARENTHESIZED_EXPRESSION_SYNTAX:
      return bind_parenthesized_expression(
          binder,
          (struct ParenthesizedExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_UNARY_EXPRESSION_SYNTAX:
      return bind_unary_expression(
          binder,
          (struct UnaryExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_NAME_EXPRESSION_SYNTAX:
      return bind_name_expression(binder, (struct NameExpressionSyntax*)syntax);
    case EXPRESSION_SYNTAX_KIND_ASSIGNMENT_EXPRESSION_SYNTAX:
      return bind_assignment_expression(
          binder,
          (struct AssignmentExpressionSyntax*)syntax);
  }
}

static struct BoundExpression* bind_literal_expression(
    struct Binder* binder,
    struct LiteralExpressionSyntax* syntax)
{
  (void)binder;
  struct Object* value = syntax->value;
  struct Object* actual_value;
  if (value->kind == OBJECT_KIND_NULL)
  {
    actual_value = OBJECT_INTEGER(0);
  }
  else
  {
    actual_value = value;
  }
  return (struct BoundExpression*)bound_literal_expression_new(actual_value);
}

static struct BoundExpression* bind_binary_expression(
    struct Binder* binder,
    struct BinaryExpressionSyntax* syntax)
{
  struct BoundExpression* bound_left = bind_expression(binder, syntax->left);
  struct BoundExpression* bound_right = bind_expression(binder, syntax->right);
  struct BoundBinaryOperator* bound_operator = bind_binary_operator(
      syntax->operator_token->kind,
      bound_expression_get_type(bound_left),
      bound_expression_get_type(bound_right));
  if (!bound_operator)
  {
    diagnostic_bag_report_undefined_binary_operator(
        binder->diagnostics,
        syntax_token_get_span(syntax->operator_token),
        syntax->operator_token->text,
        bound_expression_get_type(bound_left),
        bound_expression_get_type(bound_right));
    return bound_left;
  }
  return (struct BoundExpression*)
      bound_binary_expression_new(bound_left, bound_operator, bound_right);
}

static struct BoundExpression* bind_parenthesized_expression(
    struct Binder* binder,
    struct ParenthesizedExpressionSyntax* syntax)
{
  return bind_expression(binder, syntax->expression);
}

static struct BoundExpression* bind_unary_expression(
    struct Binder* binder,
    struct UnaryExpressionSyntax* syntax)
{
  struct BoundExpression* bound_operand
      = bind_expression(binder, syntax->operand);
  struct BoundUnaryOperator* bound_operator = bind_unary_operator(
      syntax->operator_token->kind,
      bound_expression_get_type(bound_operand));
  if (!bound_operator)
  {
    diagnostic_bag_report_undefined_unary_operator(
        binder->diagnostics,
        syntax_token_get_span(syntax->operator_token),
        syntax->operator_token->text,
        bound_expression_get_type(bound_operand));
    return bound_operand;
  }
  return (struct BoundExpression*)bound_unary_expression_new(
      bound_operator,
      bound_operand);
}

static struct BoundExpression* bind_name_expression(
    struct Binder* binder,
    struct NameExpressionSyntax* syntax)
{
  sds name = syntax->identifier_token->text;

  struct VariableSymbol* variable = NULL;
  for (long i = 0; i < binder->variables->num_entries; ++i)
  {
    if (binder->variables->entries[i].symbol
        && sdscmp(name, binder->variables->entries[i].symbol->name) == 0)
    {
      variable = binder->variables->entries[i].symbol;
    }
  }

  if (!variable)
  {
    diagnostic_bag_report_undefined_name(
        binder->diagnostics,
        syntax_token_get_span(syntax->identifier_token),
        name);
    return (struct BoundExpression*)bound_literal_expression_new(
        OBJECT_INTEGER(0));
  }

  return (struct BoundExpression*)bound_variable_expression_new(variable);
}

static struct BoundExpression* bind_assignment_expression(
    struct Binder* binder,
    struct AssignmentExpressionSyntax* syntax)
{
  sds name = syntax->identifier_token->text;
  struct BoundExpression* bound_expression
      = bind_expression(binder, syntax->expression);
  // HACK
  for (long i = 0; i < binder->variables->num_entries; ++i)
  {
    if (binder->variables->entries[i].symbol
        && sdscmp(name, binder->variables->entries[i].symbol->name) == 0)
    {
      // remove existing variable
      binder->variables->entries[i].value = OBJECT_NULL();
      break;
    }
  }
  struct VariableSymbol* variable
      = variable_symbol_new(name, bound_expression_get_type(bound_expression));
  variable_store_insert_or_assign(binder->variables, variable, OBJECT_NULL());
  return (struct BoundExpression*)bound_assignment_expression_new(
      variable,
      bound_expression);
}
