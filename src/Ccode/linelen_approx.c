/* This is a slightly modified version of the
   program from figure 9.10 of the book of A. Moffat */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double approx_line_length(double (*f)(double),
                                 double start,double stop,
                                 unsigned long k)
{
  double total = 0.0, x1 = start, fx1 = f(start);
  const double dx = (stop - start)/k, dx_square = dx * dx;
  unsigned long i;

  for (i = 1; i <= k; i++)
  {
    double fx2 = f(x1 + dx),  /* x1 + dx = start + d * i */
           dy = fx2 - fx1;

    total += sqrt(dx_square + dy * dy);
    fx1 = fx2;  /* reuse fx2 as fx1 in next iteration */
    x1 += dx;  /* x1 becomes start + d * i */
  }
  return total;
}

static double wave_fun(double x)
{
  return sin(5.0 * x) + cos(10.0 * x) + (x * x)/10.0;
}

int main(int argc, char *argv[])
{
  long k, maxsteps;
  if (argc != 2 || sscanf(argv[1],"%ld",&maxsteps) != 1 ||
      maxsteps < 1)
  {
    fprintf(stderr,"Usage: %s <maxsteps>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  for (k = 1; k <= maxsteps; k *= 10)
    printf("k = %7ld, line length %.6f\n",k,
            approx_line_length(wave_fun,0.0,10.0,k));
  return EXIT_SUCCESS;
}
