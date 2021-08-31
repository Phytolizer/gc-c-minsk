add_executable(mc Lexer.c Main.c Object.c Parser.c SyntaxKind.c SyntaxToken.c)
target_link_libraries(mc PRIVATE ${BDWGC} sds)
