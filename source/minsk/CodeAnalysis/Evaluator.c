#include "minsk/CodeAnalysis/Evaluator.h"

#include <assert.h>
#include <stdio.h>

#include <minsk/CodeAnalysis/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/UnaryExpressionSyntax.h>

static int evaluate_expression(struct ExpressionSyntax* root);

struct Evaluator* evaluator_new(struct ExpressionSyntax* root)
{
  struct Evaluator* evaluator = mc_malloc(sizeof(struct Evaluator));
  evaluator->root = root;
  return evaluator;
}

int evaluator_evaluate(struct Evaluator* evaluator)
{
  return evaluate_expression(evaluator->root);
}

static int evaluate_expression(struct ExpressionSyntax* root)
{
  switch (root->kind)
  {
    case EXPRESSION_SYNTAX_KIND_LITERAL_EXPRESSION_SYNTAX:
      return OBJECT_AS_INTEGER(
                 ((struct LiteralExpressionSyntax*)root)->literal_token->value)
          ->value;
    case EXPRESSION_SYNTAX_KIND_PARENTHESIZED_EXPRESSION_SYNTAX:
      return evaluate_expression(
          ((struct ParenthesizedExpressionSyntax*)root)->expression);
    case EXPRESSION_SYNTAX_KIND_UNARY_EXPRESSION_SYNTAX:
      {
        int operand = evaluate_expression(
            ((struct UnaryExpressionSyntax*)root)->operand);
        switch (((struct UnaryExpressionSyntax*)root)->operator_token->kind)
        {
          case SYNTAX_KIND_PLUS_TOKEN:
            return operand;
          case SYNTAX_KIND_MINUS_TOKEN:
            return -operand;
          default:
            fprintf(
                stderr,
                "unexpected unary operator %s\n",
                SYNTAX_KINDS[((struct UnaryExpressionSyntax*)root)
                                 ->operator_token->kind]);
            assert(false && "unexpected unary operator");
        }
      }
    case EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX:
      {
        int left
            = evaluate_expression(((struct BinaryExpressionSyntax*)root)->left);
        int right = evaluate_expression(
            ((struct BinaryExpressionSyntax*)root)->right);

        switch (((struct BinaryExpressionSyntax*)root)->operator_token->kind)
        {
          case SYNTAX_KIND_PLUS_TOKEN:
            return left + right;
          case SYNTAX_KIND_MINUS_TOKEN:
            return left - right;
          case SYNTAX_KIND_STAR_TOKEN:
            return left * right;
          case SYNTAX_KIND_SLASH_TOKEN:
            return left / right;
          default:
            fprintf(
                stderr,
                "unexpected binary operator %s\n",
                SYNTAX_KINDS[((struct BinaryExpressionSyntax*)root)
                                 ->operator_token->kind]);
            assert(false && "unexpected binary operator");
        }
      }
    default:
      fprintf(
          stderr,
          "unhandled syntax node %s\n",
          SYNTAX_KINDS[expression_syntax_get_kind(root)]);
      assert(false && "unhandled syntax node");
  }
}
