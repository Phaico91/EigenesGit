#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  short int n, m, *ptr;

  m = 15;      /* store 15 in var m of 2 bytes */
  ptr = &n;    /* store address of n, assuming 4-byte pointers */
  *ptr = 42;   /* contents of var. referred to by ptr becomes 42 */
  printf("n=%hd,m=%hd\n",n,m);
  return EXIT_SUCCESS;
}
