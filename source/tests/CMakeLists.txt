add_executable(MinskTests TauMain.c TestLexer.c)
target_link_libraries(MinskTests PUBLIC minsk Tau)
