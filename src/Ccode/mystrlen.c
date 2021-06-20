#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t mystrlen(const char *s)
{
  const char *sp; /* const as string will not change */

  for (sp = s; *sp != '\0'; sp++)
    /* Nothing */ ;
  return (size_t) (sp - s);
}

void encode_inplace(char *s)
{
  char *sp;
  for (sp = s; *sp != '\0'; sp++)
    *sp = *sp + 1; /* => sp cannot be const =>
                         s cannot be const */
}

int main(void)
{
  char *s;

  printf("filename \"%s\" has length %lu\n",__FILE__,
         mystrlen(__FILE__));
  s = strdup(__FILE__); /* dup as it will be modified */
  printf("encode_inplace(%s)=",s);
  encode_inplace(s);
  printf("%s\n",s);
  free(s);
  return EXIT_SUCCESS;
}
