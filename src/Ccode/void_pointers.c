#include <stdlib.h>
#include <stdio.h>

int main()
{
  void *v;
  char *s = v;

  /* fails v++;*/
  /* fails printf("sizeof *v=%lu\n",sizeof *v); */
  exit(EXIT_FAILURE);
}
