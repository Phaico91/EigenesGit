/* This is a slightly modified version of the
   program from figure 9.12 of the book of A. Moffat */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

static double bisection(double (*f)(double),double x1, double x2, double eps,
                        unsigned long maxiterations)
{
  double fx1 = f(x1), fx2 = f(x2);
  unsigned long iter;

  for (iter = 0; x2 - x1 > eps && iter < maxiterations; iter++)
  {
    double mid = (x1 + x2)/2.0;
    double fmid = f(mid);
    assert((fx1 < 0 && fx2 > 0) || (fx1 > 0 && fx2 < 0));
    if (fmid * fx1 > 0) /* same sign */
    {
      x1 = mid;
      fx1 = fmid;
    } else
    {
      assert(fmid * fx2 > 0);
      x2 = mid;
      fx2 = fmid;
    }
  }
  return (x1 + x2)/2.0;
}

static double my_function(double x)
{
  return 10 - x * x;
}

int main(int argc, char *argv[])
{
  double x_where_fx_is_0,
         eps;

  if (argc != 2 || sscanf(argv[1],"%lf",&eps) != 1 || eps < 0.0)
  {
    fprintf(stderr,"Usage: %s <eps>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  x_where_fx_is_0 = bisection(my_function,-2,5,eps,1000);
  printf("eps = %.2e, x_where_fx_is_0=%.10f, %.10f\n",eps,
         x_where_fx_is_0,my_function(x_where_fx_is_0));
  return EXIT_SUCCESS;
}
