/* This is a slightly modified version of the
   program from figure 9.8 of the book of A. Moffat */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

static double area_MCE(bool (*inside)(double,double),
                       unsigned long n)
{
  unsigned long i = 0, j;

  for (j = 0; j < n; j++)
  {
    double a = drand48(), b = drand48();/* random in [0,1)*/
    if (inside(a,b))
      i++;
  }
  return (double) i/n;
}

static bool inside_area(double a,double b)
{
  assert(a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0);
  return (1.0 <= a + b && a * a + b * b <= 1.0) ? true : false;
}

int main(int argc, char *argv[])
{
  long max_n;
  unsigned long n;

  if (argc != 2 || sscanf(argv[1],"%ld",&max_n) != 1
                || max_n < 1)
  {
    fprintf(stderr,"Usage: %s <maxsteps>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  srand48(time(NULL)); /* seed is changing each second */
  for (n = 1; n <= (unsigned long) max_n; n *= 10)
  {
    printf("n = %9lu, a = %.6f\n",n,area_MCE(inside_area,n));
  }
  return EXIT_SUCCESS;
}
