add_executable(
  MinskTests
  DoctestMain.cpp CodeAnalysis/Syntax/EvaluatorTests.cpp
  CodeAnalysis/Syntax/LexerTests.cpp CodeAnalysis/Syntax/ParserTests.cpp
  CodeAnalysis/Syntax/SyntaxFactsTests.cpp)
target_link_libraries(MinskTests PUBLIC sds common minsk doctest)
