add_executable(MinskTests DoctestMain.cpp CodeAnalysis/Syntax/LexerTests.cpp)
target_link_libraries(MinskTests PUBLIC minsk doctest)
