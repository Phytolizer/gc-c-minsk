#include "Evaluator.h"

#include <assert.h>
#include <stdio.h>

#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>

#include "Binding/BoundAssignmentExpression.h"
#include "Binding/BoundBinaryExpression.h"
#include "Binding/BoundBinaryOperatorKind.h"
#include "Binding/BoundExpression.h"
#include "Binding/BoundLiteralExpression.h"
#include "Binding/BoundUnaryExpression.h"
#include "Binding/BoundUnaryOperatorKind.h"
#include "Binding/BoundVariableExpression.h"

static struct Object* evaluate_expression(
    struct Evaluator* evaluator,
    struct BoundExpression* root);

struct Evaluator* evaluator_new(
    struct BoundExpression* root,
    struct VariableStore* variables)
{
  struct Evaluator* evaluator = mc_malloc(sizeof(struct Evaluator));
  evaluator->root = root;
  evaluator->variables = variables;
  return evaluator;
}

struct Object* evaluator_evaluate(struct Evaluator* evaluator)
{
  return evaluate_expression(evaluator, evaluator->root);
}

static struct Object* evaluate_expression(
    struct Evaluator* evaluator,
    struct BoundExpression* root)
{
  switch (bound_expression_get_kind(root))
  {
    case BOUND_NODE_KIND_LITERAL_EXPRESSION:
      return ((struct BoundLiteralExpression*)root)->value;
    case BOUND_NODE_KIND_UNARY_EXPRESSION:
      {
        struct Object* operand = evaluate_expression(
            evaluator,
            ((struct BoundUnaryExpression*)root)->operand);
        switch (((struct BoundUnaryExpression*)root)->op->kind)
        {
          case BOUND_UNARY_OPERATOR_KIND_IDENTITY:
            return operand;
          case BOUND_UNARY_OPERATOR_KIND_NEGATION:
            return OBJECT_INTEGER(-OBJECT_AS_INTEGER(operand)->value);
          case BOUND_UNARY_OPERATOR_KIND_LOGICAL_NEGATION:
            return OBJECT_BOOLEAN(!OBJECT_AS_BOOLEAN(operand)->value);
        }
      }
    case BOUND_NODE_KIND_BINARY_EXPRESSION:
      {
        struct Object* left = evaluate_expression(
            evaluator,
            ((struct BoundBinaryExpression*)root)->left);
        struct Object* right = evaluate_expression(
            evaluator,
            ((struct BoundBinaryExpression*)root)->right);

        switch (((struct BoundBinaryExpression*)root)->op->kind)
        {
          case BOUND_BINARY_OPERATOR_KIND_ADDITION:
            return OBJECT_INTEGER(
                OBJECT_AS_INTEGER(left)->value
                + OBJECT_AS_INTEGER(right)->value);
          case BOUND_BINARY_OPERATOR_KIND_SUBTRACTION:
            return OBJECT_INTEGER(
                OBJECT_AS_INTEGER(left)->value
                - OBJECT_AS_INTEGER(right)->value);
          case BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION:
            return OBJECT_INTEGER(
                OBJECT_AS_INTEGER(left)->value
                * OBJECT_AS_INTEGER(right)->value);
          case BOUND_BINARY_OPERATOR_KIND_DIVISION:
            return OBJECT_INTEGER(
                OBJECT_AS_INTEGER(left)->value
                / OBJECT_AS_INTEGER(right)->value);
          case BOUND_BINARY_OPERATOR_KIND_LOGICAL_AND:
            return OBJECT_BOOLEAN(
                OBJECT_AS_BOOLEAN(left)->value
                && OBJECT_AS_BOOLEAN(right)->value);
          case BOUND_BINARY_OPERATOR_KIND_LOGICAL_OR:
            return OBJECT_BOOLEAN(
                OBJECT_AS_BOOLEAN(left)->value
                || OBJECT_AS_BOOLEAN(right)->value);
          case BOUND_BINARY_OPERATOR_KIND_EQUALITY:
            return OBJECT_BOOLEAN(objects_equal(left, right));
          case BOUND_BINARY_OPERATOR_KIND_INEQUALITY:
            return OBJECT_BOOLEAN(!objects_equal(left, right));
        }
      }
    case BOUND_EXPRESSION_KIND_BOUND_VARIABLE_EXPRESSION:
      return *variable_store_lookup(
          evaluator->variables,
          ((struct BoundVariableExpression*)root)->name);
    case BOUND_EXPRESSION_KIND_BOUND_ASSIGNMENT_EXPRESSION:
      {
        struct Object* value = evaluate_expression(
            evaluator,
            ((struct BoundAssignmentExpression*)root)->expression);
        variable_store_insert(
            evaluator->variables,
            ((struct BoundAssignmentExpression*)root)->name,
            value);
        return value;
      }
    default:
      fprintf(
          stderr,
          "unhandled syntax node %s\n",
          BOUND_NODE_KINDS[bound_expression_get_kind(root)]);
      assert(false && "unhandled syntax node");
  }
}
