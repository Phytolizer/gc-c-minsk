add_executable(
  mc
  BinaryExpressionSyntax.c
  ExpressionSyntax.c
  Lexer.c
  Main.c
  NumberExpressionSyntax.c
  Object.c
  Parser.c
  SyntaxKind.c
  SyntaxNode.c
  SyntaxToken.c
  SyntaxTree.c)
target_link_libraries(mc PRIVATE ${BDWGC} sds)
