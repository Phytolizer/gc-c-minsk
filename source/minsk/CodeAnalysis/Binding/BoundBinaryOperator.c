#include "BoundBinaryOperator.h"

#include <common/List.h>

DECLARE_NAMED_LIST(BoundBinaryOperatorList, struct BoundBinaryOperator);

extern struct BoundBinaryOperator* bind_binary_operator(
    enum SyntaxKind syntax_kind,
    enum ObjectKind left_type,
    enum ObjectKind right_type)
{
  static struct BoundBinaryOperatorList* list = NULL;
  if (!list)
  {
    list = mc_malloc(sizeof(struct BoundBinaryOperatorList));
    LIST_INIT(list);
    LIST_PUSH(
        list,
        ((struct BoundBinaryOperator){
            .syntax_kind = SYNTAX_KIND_PLUS_TOKEN,
            .kind = BOUND_BINARY_OPERATOR_KIND_ADDITION,
            .left_type = OBJECT_KIND_INTEGER,
            .right_type = OBJECT_KIND_INTEGER,
            .result_type = OBJECT_KIND_INTEGER,
        }));
    LIST_PUSH(
        list,
        ((struct BoundBinaryOperator){
            .syntax_kind = SYNTAX_KIND_MINUS_TOKEN,
            .kind = BOUND_BINARY_OPERATOR_KIND_SUBTRACTION,
            .left_type = OBJECT_KIND_INTEGER,
            .right_type = OBJECT_KIND_INTEGER,
            .result_type = OBJECT_KIND_INTEGER,
        }));
    LIST_PUSH(
        list,
        ((struct BoundBinaryOperator){
            .syntax_kind = SYNTAX_KIND_STAR_TOKEN,
            .kind = BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION,
            .left_type = OBJECT_KIND_INTEGER,
            .right_type = OBJECT_KIND_INTEGER,
            .result_type = OBJECT_KIND_INTEGER,
        }));
    LIST_PUSH(
        list,
        ((struct BoundBinaryOperator){
            .syntax_kind = SYNTAX_KIND_SLASH_TOKEN,
            .kind = BOUND_BINARY_OPERATOR_KIND_DIVISION,
            .left_type = OBJECT_KIND_INTEGER,
            .right_type = OBJECT_KIND_INTEGER,
            .result_type = OBJECT_KIND_INTEGER,
        }));
    LIST_PUSH(
        list,
        ((struct BoundBinaryOperator){
            .syntax_kind = SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN,
            .kind = BOUND_BINARY_OPERATOR_KIND_LOGICAL_AND,
            .left_type = OBJECT_KIND_BOOLEAN,
            .right_type = OBJECT_KIND_BOOLEAN,
            .result_type = OBJECT_KIND_BOOLEAN,
        }));
    LIST_PUSH(
        list,
        ((struct BoundBinaryOperator){
            .syntax_kind = SYNTAX_KIND_PIPE_PIPE_TOKEN,
            .kind = BOUND_BINARY_OPERATOR_KIND_LOGICAL_OR,
            .left_type = OBJECT_KIND_BOOLEAN,
            .right_type = OBJECT_KIND_BOOLEAN,
            .result_type = OBJECT_KIND_BOOLEAN,
        }));
  }

  for (long i = 0; i < list->length; ++i)
  {
    if (list->data[i].syntax_kind == syntax_kind
        && list->data[i].left_type == left_type
        && list->data[i].right_type == right_type)
    {
      return &list->data[i];
    }
  }
  return NULL;
}
