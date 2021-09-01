add_library(
  minsk SHARED
  CodeAnalysis/Evaluator.c
  CodeAnalysis/Binding/Binder.c
  CodeAnalysis/Binding/BoundBinaryExpression.c
  CodeAnalysis/Binding/BoundBinaryOperator.c
  CodeAnalysis/Binding/BoundExpression.c
  CodeAnalysis/Binding/BoundLiteralExpression.c
  CodeAnalysis/Binding/BoundNode.c
  CodeAnalysis/Binding/BoundUnaryExpression.c
  CodeAnalysis/Binding/BoundUnaryOperator.c
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
  CodeAnalysis/Syntax/UnaryExpressionSyntax.c)
target_link_libraries(minsk PUBLIC ${BDWGC} sds common)
