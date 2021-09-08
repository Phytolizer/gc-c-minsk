#include "minsk/CodeAnalysis/Syntax/AssignmentExpressionSyntax.h"

#include "minsk/CodeAnalysis/Syntax/ExpressionSyntax.h"

struct AssignmentExpressionSyntax* assignment_expression_new(struct SyntaxToken* identifier_token,
                                                             struct SyntaxToken* equals_token,
                                                             struct ExpressionSyntax* expression)
{
    struct AssignmentExpressionSyntax* syntax = mc_malloc(sizeof(struct AssignmentExpressionSyntax));
    expression_syntax_init((struct ExpressionSyntax*)syntax, EXPRESSION_SYNTAX_KIND_ASSIGNMENT_EXPRESSION_SYNTAX);
    syntax->identifier_token = identifier_token;
    syntax->equals_token = equals_token;
    syntax->expression = expression;
    return syntax;
}

enum SyntaxKind assignment_expression_syntax_get_kind(struct AssignmentExpressionSyntax* syntax)
{
    (void)syntax;
    return SYNTAX_KIND_ASSIGNMENT_EXPRESSION;
}

struct SyntaxNodeList* assignment_expression_syntax_get_children(struct AssignmentExpressionSyntax* syntax)
{
    struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
    LIST_INIT(children);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->identifier_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->equals_token);
    LIST_PUSH(children, (struct SyntaxNode*)syntax->expression);
    return children;
}
