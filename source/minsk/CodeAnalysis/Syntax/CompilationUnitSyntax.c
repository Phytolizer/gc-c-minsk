#include <minsk/CodeAnalysis/Syntax/CompilationUnitSyntax.h>

struct CompilationUnitSyntax* compilation_unit_syntax_new(
    struct ExpressionSyntax* expression,
    struct SyntaxToken* end_of_file_token)
{
  struct CompilationUnitSyntax* unit = mc_malloc(sizeof(struct CompilationUnitSyntax));
  unit->base.kind = SYNTAX_NODE_KIND_COMPILATION_UNIT;
  unit->expression = expression;
  unit->end_of_file_token = end_of_file_token;
  return unit;
}

enum SyntaxKind compilation_unit_syntax_get_kind(struct CompilationUnitSyntax* unit)
{
  (void)unit;
  return SYNTAX_KIND_COMPILATION_UNIT;
}

struct SyntaxNodeList* compilation_unit_syntax_get_children(
    struct CompilationUnitSyntax* unit)
{
  struct SyntaxNodeList* children = mc_malloc(sizeof(struct SyntaxNodeList));
  LIST_INIT(children);
  LIST_PUSH(children, (struct SyntaxNode*)unit->expression);
  LIST_PUSH(children, (struct SyntaxNode*)unit->end_of_file_token);
  return children;
}
