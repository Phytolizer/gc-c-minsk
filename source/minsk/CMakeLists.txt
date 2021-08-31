add_executable(
  mc
  BinaryExpressionSyntax.c
  Evaluator.c
  ExpressionSyntax.c
  Lexer.c
  Main.c
  NumberExpressionSyntax.c
  Object.c
  ParenthesizedExpressionSyntax.c
  Parser.c
  SyntaxKind.c
  SyntaxNode.c
  SyntaxToken.c
  SyntaxTree.c)
target_link_libraries(mc PRIVATE ${BDWGC} sds)
