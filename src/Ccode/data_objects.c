#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int x;
int main(void)
{
  int i = 20;
  {int x; x = i + 7; x--;}
  x++;
  return 0;
}

int f(int n)
{
  int a, *p;
  a = 1;
  p = (int *) malloc(sizeof(int));
  *p = n;
  return *p + a;
}

int *append_int(int *a,unsigned long m,
                const int *b,unsigned long n)
{
  assert(a + m <= b || b + n <= a);/* no overlaps */
  if (n > 0)
  {
    a = realloc(a,(m+n) * sizeof *a);
    memcpy(a+m,b,n * sizeof *b);/* copy n * sizeof *b
                                   bytes from b to a+m */
  }
  return a;
}

#ifdef INCLUDE_THIS_CODE
int *append_int(int *a,unsigned long m,
                const int *b,unsigned long n)
lstend*/

    a = realloc(a,(m+n) * sizeof *a);
    memcpy(a+m,b,n * sizeof *b);
lstend*/
#endif
