#include "minsk/CodeAnalysis/Evaluator.h"

#include <assert.h>
#include <stdio.h>

#include <minsk/CodeAnalysis/Binding/BoundExpression.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>

#include "Binding/BoundBinaryExpression.h"
#include "Binding/BoundBinaryOperatorKind.h"
#include "Binding/BoundLiteralExpression.h"
#include "Binding/BoundUnaryExpression.h"
#include "Binding/BoundUnaryOperatorKind.h"

static struct Object* evaluate_expression(struct BoundExpression* root);

struct Evaluator* evaluator_new(struct BoundExpression* root)
{
  struct Evaluator* evaluator = mc_malloc(sizeof(struct Evaluator));
  evaluator->root = root;
  return evaluator;
}

struct Object* evaluator_evaluate(struct Evaluator* evaluator)
{
  return evaluate_expression(evaluator->root);
}

static struct Object* evaluate_expression(struct BoundExpression* root)
{
  switch (bound_expression_get_kind(root))
  {
    case BOUND_NODE_KIND_LITERAL_EXPRESSION:
      return ((struct BoundLiteralExpression*)root)->value;
    case BOUND_NODE_KIND_UNARY_EXPRESSION:
      {
        struct Object* operand = evaluate_expression(
            ((struct BoundUnaryExpression*)root)->operand);
        switch (((struct BoundUnaryExpression*)root)->operator_kind)
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
        struct Object* left
            = evaluate_expression(((struct BoundBinaryExpression*)root)->left);
        struct Object* right
            = evaluate_expression(((struct BoundBinaryExpression*)root)->right);

        switch (((struct BoundBinaryExpression*)root)->operator_kind)
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
        }
      }
    default:
      fprintf(
          stderr,
          "unhandled syntax node %s\n",
          BOUND_NODE_KINDS[bound_expression_get_kind(root)]);
      assert(false && "unhandled syntax node");
  }
}
