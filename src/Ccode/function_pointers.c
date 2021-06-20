#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int f(void)
{
  return 3;
}

double *g(int v)
{
  double *d
    = malloc(sizeof *d);
  *d = sqrt((double) v);
  return d;
}

int main(void)
{
  int i, (*fp)(void);
  double *d, * (*gp)(int);

  fp = f;/* point to prog. segment */
  gp = g;/* point to prog. segment */
  i = fp(); /* implicitely call f */
  i++;
  d = gp(8); /* implicitely call g */
  printf("i=%d, gp(8)=%.2f\n",i,*d);
  free(d);
  return EXIT_SUCCESS;
}
