add_executable(
  mc
  CodeAnalysis/BinaryExpressionSyntax.c
  CodeAnalysis/Evaluator.c
  CodeAnalysis/ExpressionSyntax.c
  CodeAnalysis/Lexer.c
  CodeAnalysis/LiteralExpressionSyntax.c
  CodeAnalysis/ParenthesizedExpressionSyntax.c
  CodeAnalysis/Parser.c
  CodeAnalysis/SyntaxKind.c
  CodeAnalysis/SyntaxNode.c
  CodeAnalysis/SyntaxToken.c
  CodeAnalysis/SyntaxTree.c
  Main.c)
target_link_libraries(mc PRIVATE ${BDWGC} sds common)
