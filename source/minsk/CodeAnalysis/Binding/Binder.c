#include "minsk/CodeAnalysis/Binding/Binder.h"

#include <assert.h>
#include <stdio.h>

#include <IncludeMe.h>
#include <common/Nullable.h>
#include <common/Object.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>

#include "BoundBinaryExpression.h"
#include "BoundLiteralExpression.h"
#include "BoundUnaryExpression.h"

DECLARE_NULLABLE(NullableBoundUnaryOperatorKind, enum BoundUnaryOperatorKind);
DECLARE_NULLABLE(NullableBoundBinaryOperatorKind, enum BoundBinaryOperatorKind);

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

static struct NullableBoundUnaryOperatorKind bind_unary_operator_kind(
    enum SyntaxKind kind,
    enum ObjectKind operand_type);
static struct NullableBoundBinaryOperatorKind bind_binary_operator_kind(
    enum SyntaxKind kind,
    enum ObjectKind left_type,
    enum ObjectKind right_type);

struct Binder* binder_new(void)
{
  struct Binder* binder = mc_malloc(sizeof(struct Binder));
  binder->diagnostics = mc_malloc(sizeof(struct StringList));
  LIST_INIT(binder->diagnostics);
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
  }
}

static struct BoundExpression* bind_literal_expression(
    struct Binder* binder,
    struct LiteralExpressionSyntax* syntax)
{
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
  struct NullableBoundBinaryOperatorKind bound_operator_kind
      = bind_binary_operator_kind(
          syntax->operator_token->kind,
          bound_expression_get_type(bound_left),
          bound_expression_get_type(bound_right));
  if (bound_operator_kind.discriminator
      == NullableBoundBinaryOperatorKind_NOTHING)
  {
    LIST_PUSH(
        binder->diagnostics,
        sdscatprintf(
            sdsempty(),
            "Binary operator '%s' is not defined for types %s and %s.",
            syntax->operator_token->text,
            OBJECT_KINDS[bound_expression_get_type(bound_left)],
            OBJECT_KINDS[bound_expression_get_type(bound_right)]));
    return bound_left;
  }
  return (struct BoundExpression*)bound_binary_expression_new(
      bound_left,
      bound_operator_kind.value,
      bound_right);
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
  struct NullableBoundUnaryOperatorKind bound_operator_kind
      = bind_unary_operator_kind(
          syntax->operator_token->kind,
          bound_expression_get_type(bound_operand));
  if (bound_operator_kind.discriminator
      == NullableBoundUnaryOperatorKind_NOTHING)
  {
    LIST_PUSH(
        binder->diagnostics,
        sdscatprintf(
            sdsempty(),
            "Unary operator '%s' is not defined for type %s.",
            syntax->operator_token->text,
            OBJECT_KINDS[bound_expression_get_type(bound_operand)]));
    return bound_operand;
  }
  return (struct BoundExpression*)bound_unary_expression_new(
      bound_operator_kind.value,
      bound_operand);
}

static struct NullableBoundUnaryOperatorKind bind_unary_operator_kind(
    enum SyntaxKind kind,
    enum ObjectKind operand_type)
{
  switch (operand_type)
  {
    case OBJECT_KIND_INTEGER:
      switch (kind)
      {
        case SYNTAX_KIND_PLUS_TOKEN:
          return SOMETHING(
              NullableBoundUnaryOperatorKind,
              BOUND_UNARY_OPERATOR_KIND_IDENTITY);
        case SYNTAX_KIND_MINUS_TOKEN:
          return SOMETHING(
              NullableBoundUnaryOperatorKind,
              BOUND_UNARY_OPERATOR_KIND_NEGATION);
        default:
          break;
      }
      break;
    case OBJECT_KIND_BOOLEAN:
      switch (kind)
      {
        case SYNTAX_KIND_BANG_TOKEN:
          return SOMETHING(
              NullableBoundUnaryOperatorKind,
              BOUND_UNARY_OPERATOR_KIND_LOGICAL_NEGATION);
        default:
          break;
      }
    default:
      break;
  }

  return NOTHING(NullableBoundUnaryOperatorKind);
}

static struct NullableBoundBinaryOperatorKind bind_binary_operator_kind(
    enum SyntaxKind kind,
    enum ObjectKind left_type,
    enum ObjectKind right_type)
{
  if (left_type == OBJECT_KIND_INTEGER && right_type == OBJECT_KIND_INTEGER)
  {
    switch (kind)
    {
      case SYNTAX_KIND_PLUS_TOKEN:
        return SOMETHING(
            NullableBoundBinaryOperatorKind,
            BOUND_BINARY_OPERATOR_KIND_ADDITION);
      case SYNTAX_KIND_MINUS_TOKEN:
        return SOMETHING(
            NullableBoundBinaryOperatorKind,
            BOUND_BINARY_OPERATOR_KIND_SUBTRACTION);
      case SYNTAX_KIND_STAR_TOKEN:
        return SOMETHING(
            NullableBoundBinaryOperatorKind,
            BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION);
      case SYNTAX_KIND_SLASH_TOKEN:
        return SOMETHING(
            NullableBoundBinaryOperatorKind,
            BOUND_BINARY_OPERATOR_KIND_DIVISION);
      default:
        break;
    }
  }
  if (left_type == OBJECT_KIND_BOOLEAN && right_type == OBJECT_KIND_BOOLEAN)
  {
    switch (kind)
    {
      case SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN:
        return SOMETHING(
            NullableBoundBinaryOperatorKind,
            BOUND_BINARY_OPERATOR_KIND_LOGICAL_AND);
      case SYNTAX_KIND_PIPE_PIPE_TOKEN:
        return SOMETHING(
            NullableBoundBinaryOperatorKind,
            BOUND_BINARY_OPERATOR_KIND_LOGICAL_OR);
      default:
        break;
    }
  }
  return NOTHING(NullableBoundBinaryOperatorKind);
}
