#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <gc.h>

#include "Lexer.h"
#include "sds.h"

sds input_line(const char* prompt)
{
  printf("%s", prompt);
  sds line = sdsempty();
  while (true)
  {
    char c = fgetc(stdin);
    if (c == EOF)
    {
      return NULL;
    }
    if (c == '\n' || c == '\0')
    {
      break;
    }
    line = sdscatlen(line, &c, 1);
  }
  return line;
}

int main(void)
{
  while (true)
  {
    sds line = input_line("> ");
    if (!line)
    {
      printf("\n");
      break;
    }

    struct Lexer* lexer = lexer_new(line);

    while (true)
    {
      struct SyntaxToken* token = lexer_next_token(lexer);
      if (token->kind == SYNTAX_KIND_END_OF_FILE_TOKEN)
      {
        break;
      }
      printf("%s: '%s'", SYNTAX_KINDS[token->kind], token->text);
      if (!OBJECT_IS_NULL(token->value))
      {
        printf(" %s", object_to_string(token->value));
      }
      printf("\n");
    }
  }
  return 0;
}
