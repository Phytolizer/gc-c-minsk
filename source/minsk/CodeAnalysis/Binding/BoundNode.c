#include "minsk-private/CodeAnalysis/Binding/BoundNode.h"

#include <minsk-private/CodeAnalysis/Binding/BoundBinaryExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundLiteralExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundStatement.h>
#include <minsk-private/CodeAnalysis/Binding/BoundUnaryExpression.h>

struct Property
{
    sds name;
    struct Object* value;
};

DECLARE_NAMED_LIST(PropertyList, struct Property);

const char* const BOUND_NODE_KINDS[] = {
#define X(x) #x,
    BOUND_NODE_KINDS_
#undef X
};

static void pretty_print(FILE* stream, struct BoundNode* node, sds indent, bool is_last);
static const char* get_ansi_color_escape(struct BoundNode* node);
static sds get_text(struct BoundNode* node);
static struct PropertyList* get_properties(struct BoundNode* node);

enum BoundNodeKind bound_node_get_kind(struct BoundNode* node)
{
    switch (node->type)
    {
    case BOUND_NODE_TYPE_EXPRESSION:
        return bound_expression_get_kind((struct BoundExpression*)node);
    case BOUND_NODE_TYPE_STATEMENT:
        return bound_statement_get_kind((struct BoundStatement*)node);
    }
}

struct BoundNodeList* bound_node_get_children(struct BoundNode* node)
{
    switch (node->type)
    {
    case BOUND_NODE_TYPE_EXPRESSION:
        return bound_expression_get_children((struct BoundExpression*)node);
    case BOUND_NODE_TYPE_STATEMENT:
        return bound_statement_get_children((struct BoundStatement*)node);
    }
}

void bound_node_pretty_print(FILE* stream, struct BoundNode* node)
{
    pretty_print(stream, node, sdsempty(), true);
}

static void pretty_print(FILE* stream, struct BoundNode* node, sds indent, bool is_last)
{
    bool is_to_stdout = stream == stdout;
    sds marker = is_last ? sdsnew("└───") : sdsnew("├───");
    if (is_to_stdout)
    {
        fprintf(stream, "\x1b[2;37m");
    }
    fprintf(stream, "%s", indent);
    fprintf(stream, "%s", marker);
    if (is_to_stdout)
    {
        fprintf(stream, "%s", get_ansi_color_escape(node));
    }
    fprintf(stream, "%s", get_text(node));
    struct PropertyList* properties = get_properties(node);
    for (long i = 0; i < properties->length; ++i)
    {
        if (i > 0)
        {
            if (is_to_stdout)
            {
                fprintf(stream, "\x1b[2;37m");
            }
            fprintf(stream, ",");
            if (is_to_stdout)
            {
                fprintf(stream, "%s", get_ansi_color_escape(node));
            }
        }
        fprintf(stream, " ");
        struct Property* p = &properties->data[i];
        if (is_to_stdout)
        {
            fprintf(stream, "\x1b[0;33m");
        }
        fprintf(stream, "%s", p->name);
        if (is_to_stdout)
        {
            fprintf(stream, "\x1b[2;37m");
        }
        fprintf(stream, " = ");
        if (is_to_stdout)
        {
            fprintf(stream, "\x1b[2;33m");
        }
        fprintf(stream, "%s", object_to_string(p->value));
    }
    if (is_to_stdout)
    {
        fprintf(stream, "\x1b[0m");
    }

    fprintf(stream, "\n");
    indent = sdsdup(indent);
    indent = sdscat(indent, is_last ? "    " : "│   ");
    struct BoundNodeList* children = bound_node_get_children(node);
    for (long i = 0; i < children->length; i++)
    {
        pretty_print(stream, children->data[i], indent, i == children->length - 1);
    }
}

static const char* get_ansi_color_escape(struct BoundNode* node)
{
    switch (node->type)
    {
    case BOUND_NODE_TYPE_EXPRESSION:
        return "\x1b[0;34m";
    case BOUND_NODE_TYPE_STATEMENT:
        return "\x1b[0;36m";
    }
}

static sds get_text(struct BoundNode* node)
{
    enum BoundNodeKind kind = bound_node_get_kind(node);
    switch (kind)
    {
    case BOUND_NODE_KIND_BINARY_EXPRESSION:
        return sdscatfmt(
            sdsempty(), "%s_EXPRESSION", BOUND_BINARY_OPERATOR_KINDS[((struct BoundBinaryExpression*)node)->op->kind]);
    case BOUND_NODE_KIND_UNARY_EXPRESSION:
        return sdscatfmt(
            sdsempty(), "%s_EXPRESSION", BOUND_UNARY_OPERATOR_KINDS[((struct BoundUnaryExpression*)node)->op->kind]);
    default:
        return sdsnew(BOUND_NODE_KINDS[kind]);
    }
}

static struct PropertyList* get_properties(struct BoundNode* node)
{
    struct PropertyList* list = mc_malloc(sizeof(struct PropertyList));
    LIST_INIT(list);
    switch (bound_node_get_kind(node))
    {
    case BOUND_NODE_KIND_BINARY_EXPRESSION: {
        struct BoundBinaryExpression* expr = (struct BoundBinaryExpression*)node;
        LIST_PUSH(
            list,
            ((struct Property){
                .name = "type",
                .value = OBJECT_STRING(sdsnew(OBJECT_KINDS[bound_binary_expression_get_type(expr)])),
            }));
    }
    break;
    case BOUND_NODE_KIND_LITERAL_EXPRESSION: {
        struct BoundLiteralExpression* expr = (struct BoundLiteralExpression*)node;
        LIST_PUSH(
            list,
            ((struct Property){
                .name = "type",
                .value = OBJECT_STRING(sdsnew(OBJECT_KINDS[expr->value->kind])),
            }));
        LIST_PUSH(
            list,
            ((struct Property){
                .name = "value",
                .value = expr->value,
            }));
    }
    break;
    case BOUND_NODE_KIND_UNARY_EXPRESSION: {
        struct BoundUnaryExpression* expr = (struct BoundUnaryExpression*)node;
        LIST_PUSH(
            list,
            ((struct Property){
                .name = "type",
                .value = OBJECT_STRING(sdsnew(OBJECT_KINDS[bound_unary_expression_get_type(expr)])),
            }));
    }
    break;
    case BOUND_NODE_KIND_VARIABLE_EXPRESSION:
        break;
    case BOUND_NODE_KIND_ASSIGNMENT_EXPRESSION:
        break;
    case BOUND_NODE_KIND_BLOCK_STATEMENT:
        break;
    case BOUND_NODE_KIND_EXPRESSION_STATEMENT:
        break;
    case BOUND_NODE_KIND_FOR_STATEMENT:
        break;
    case BOUND_NODE_KIND_IF_STATEMENT:
        break;
    case BOUND_NODE_KIND_VARIABLE_DECLARATION:
        break;
    case BOUND_NODE_KIND_WHILE_STATEMENT:
        break;
    }
    return list;
}
