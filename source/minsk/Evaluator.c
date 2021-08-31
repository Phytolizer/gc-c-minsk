#include "Evaluator.h"

#include <assert.h>

#include "BinaryExpressionSyntax.h"
#include "NumberExpressionSyntax.h"
#include "ParenthesizedExpressionSyntax.h"

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
  if (root->kind == EXPRESSION_SYNTAX_KIND_NUMBER_EXPRESSION_SYNTAX)
  {
    return OBJECT_AS_INTEGER(
               ((struct NumberExpressionSyntax*)root)->number_token->value)
        ->value;
  }
  if (root->kind == EXPRESSION_SYNTAX_KIND_PARENTHESIZED_EXPRESSION_SYNTAX)
  {
    return evaluate_expression(
        ((struct ParenthesizedExpressionSyntax*)root)->expression);
  }
  if (root->kind == EXPRESSION_SYNTAX_KIND_BINARY_EXPRESSION_SYNTAX)
  {
    int left
        = evaluate_expression(((struct BinaryExpressionSyntax*)root)->left);
    int right
        = evaluate_expression(((struct BinaryExpressionSyntax*)root)->right);

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
        assert(false && "unexpected binary operator");
    }
  }
  assert(false && "unreachable");
}
