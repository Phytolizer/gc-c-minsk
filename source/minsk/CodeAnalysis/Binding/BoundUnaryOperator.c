#include "BoundUnaryOperator.h"

#include <common/List.h>

DECLARE_NAMED_LIST(BoundUnaryOperatorList, struct BoundUnaryOperator);

struct BoundUnaryOperator* bind_unary_operator(enum SyntaxKind syntax_kind, enum ObjectKind operand_type)
{
    struct BoundUnaryOperatorList* list = NULL;
    if (!list)
    {
        list = mc_malloc(sizeof(struct BoundUnaryOperatorList));
        LIST_INIT(list);
        LIST_PUSH(
            list,
            ((struct BoundUnaryOperator){
                .syntax_kind = SYNTAX_KIND_BANG_TOKEN,
                .kind = BOUND_UNARY_OPERATOR_KIND_LOGICAL_NEGATION,
                .operand_type = OBJECT_KIND_BOOLEAN,
                .result_type = OBJECT_KIND_BOOLEAN,
            }));
        LIST_PUSH(
            list,
            ((struct BoundUnaryOperator){
                .syntax_kind = SYNTAX_KIND_PLUS_TOKEN,
                .kind = BOUND_UNARY_OPERATOR_KIND_IDENTITY,
                .operand_type = OBJECT_KIND_INTEGER,
                .result_type = OBJECT_KIND_INTEGER,
            }));
        LIST_PUSH(
            list,
            ((struct BoundUnaryOperator){
                .syntax_kind = SYNTAX_KIND_MINUS_TOKEN,
                .kind = BOUND_UNARY_OPERATOR_KIND_NEGATION,
                .operand_type = OBJECT_KIND_INTEGER,
                .result_type = OBJECT_KIND_INTEGER,
            }));
    }

    for (long i = 0; i < list->length; ++i)
    {
        if (list->data[i].syntax_kind == syntax_kind && list->data[i].operand_type == operand_type)
        {
            return &list->data[i];
        }
    }
    return NULL;
}
