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

static int evaluate_expression(struct BoundExpression* root);

struct Evaluator* evaluator_new(struct BoundExpression* root)
{
  struct Evaluator* evaluator = mc_malloc(sizeof(struct Evaluator));
  evaluator->root = root;
  return evaluator;
}

int evaluator_evaluate(struct Evaluator* evaluator)
{
  return evaluate_expression(evaluator->root);
}

static int evaluate_expression(struct BoundExpression* root)
{
  switch (bound_expression_get_kind(root))
  {
    case BOUND_NODE_KIND_LITERAL_EXPRESSION:
      return OBJECT_AS_INTEGER(((struct BoundLiteralExpression*)root)->value)
          ->value;
    case BOUND_NODE_KIND_UNARY_EXPRESSION:
      {
        int operand = evaluate_expression(
            ((struct BoundUnaryExpression*)root)->operand);
        switch (((struct BoundUnaryExpression*)root)->operator_kind)
        {
          case BOUND_UNARY_OPERATOR_KIND_IDENTITY:
            return operand;
          case BOUND_UNARY_OPERATOR_KIND_NEGATION:
            return -operand;
        }
      }
    case BOUND_NODE_KIND_BINARY_EXPRESSION:
      {
        int left
            = evaluate_expression(((struct BoundBinaryExpression*)root)->left);
        int right
            = evaluate_expression(((struct BoundBinaryExpression*)root)->right);

        switch (((struct BoundBinaryExpression*)root)->operator_kind)
        {
          case BOUND_BINARY_OPERATOR_KIND_ADDITION:
            return left + right;
          case BOUND_BINARY_OPERATOR_KIND_SUBTRACTION:
            return left - right;
          case BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION:
            return left * right;
          case BOUND_BINARY_OPERATOR_KIND_DIVISION:
            return left / right;
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
