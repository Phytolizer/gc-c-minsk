add_executable(
  mc
  CodeAnalysis/Evaluator.c
  CodeAnalysis/Syntax/BinaryExpressionSyntax.c
  CodeAnalysis/Syntax/ExpressionSyntax.c
  CodeAnalysis/Syntax/Lexer.c
  CodeAnalysis/Syntax/LiteralExpressionSyntax.c
  CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.c
  CodeAnalysis/Syntax/Parser.c
  CodeAnalysis/Syntax/SyntaxFacts.c
  CodeAnalysis/Syntax/SyntaxKind.c
  CodeAnalysis/Syntax/SyntaxNode.c
  CodeAnalysis/Syntax/SyntaxToken.c
  CodeAnalysis/Syntax/SyntaxTree.c
  CodeAnalysis/Syntax/UnaryExpressionSyntax.c
  Main.c)
target_link_libraries(mc PRIVATE ${BDWGC} sds common)
