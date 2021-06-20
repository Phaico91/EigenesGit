#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *capitalize(char *s)
{
  s[0] = toupper(s[0]); /* modify first character */
  return s;
}

int main(void)
{
  printf("%s\n",capitalize("a cat"));
  return EXIT_SUCCESS;
}
