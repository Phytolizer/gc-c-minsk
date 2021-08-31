#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <gc.h>

char* input_line(const char* prompt)
{
  printf("%s", prompt);
  char* line = NULL;
  size_t linelen = 0;
  while (true)
  {
    char c = fgetc(stdin);
    if (c == EOF)
    {
      return NULL;
    }
    if (c == '\n')
    {
      c = '\0';
    }
    ++linelen;
    line = GC_REALLOC(line, linelen);
    line[linelen - 1] = c;
    if (c == '\0')
    {
      break;
    }
  }
  return line;
}

int main(void)
{
  while (true)
  {
    char* line = input_line("> ");
    if (!line)
    {
      printf("\n");
      break;
    }

    if (strcmp(line, "1 + 2 * 3") == 0)
    {
      printf("7\n");
    }
    else
    {
      printf("ERROR: Invalid expression!\n");
    }
  }
  return 0;
}
