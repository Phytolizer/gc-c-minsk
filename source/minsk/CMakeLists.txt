add_executable(mc Lexer.c Main.c Object.c StringUtils.c SyntaxKind.c
                  SyntaxToken.c)
target_link_libraries(mc PRIVATE ${BDWGC})
