#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* for CHAR_BIT */
#define BITS(X) (CHAR_BIT * sizeof (X))
#define ISSIGNED(TYPE) ((TYPE) -1 < 0)

int main(void)
{
  printf("this is a %lu-bit machine\n",BITS(void *));
  printf("size_t is %ssigned\n",ISSIGNED(size_t) ? "" : "un");
  return EXIT_SUCCESS;
}
