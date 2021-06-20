#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
//BEGIN{exclude}
#include <immintrin.h>
#include <sys/time.h>

unsigned long get_usecs(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec*1000000+tv.tv_usec;
}
//END{exclude}

double curvedM(double x)
{
  return 2.0 +
         sin(5.0 * x) +
         cos(10.0 * x) +
         0.1 * x * x;
}

double velocity(double t)
{
  double tsquare = t * t;
  return 3.0 * tsquare * pow(M_E,t * tsquare);
}

double velocity_antiderivative(double t)
{
  return pow(M_E,t * t * t);
}

typedef double (*FuncDouble2Double)(double);

double approx_integral_trpz(FuncDouble2Double f,double p,
                            double q, unsigned long n)
{
  double fsum = 0.0, d = (q - p)/n;
  unsigned long i;

  assert(n > 0 &&  p < q);
  for (i = 1; i < n; i++) {
    fsum += f(p + i * d);
  }
  return d * (0.5 * (f(p) + f(q)) + fsum);
}

void approx_integral_trpz_print(FuncDouble2Double f,
                                const char *fname,
                                double p, double q,
                                unsigned long maxsteps,
                                double expected)
{
  unsigned long n;
  double numerical = DBL_MAX;

  for (n = 10; n <= maxsteps; n *= 10)
  {
    numerical = approx_integral_trpz(f,p,q,n);
    printf("n = %7lu, integral(%s,%.1f,%.1f) = %.6f\n",n,fname,
           p,q,numerical);
  }
  if (numerical != DBL_MAX && expected != DBL_MAX)
  {
    const char *equality = expected == numerical ? "==" : "!=";
    printf("numerical = %.8f %s %.8f = expected\n",
           numerical,equality,expected);
  }
}

double approx_integral_mid(FuncDouble2Double f, double p,
                           double q, unsigned long n)
{
  double fsum = 0.0, d = (q - p)/n;
  unsigned long i;

  assert(n > 0 && p < q);
  for (i = 0; i < n; i++)
  {
    fsum += f(p + i * d + d/2);
  }
  return d * fsum;
}

//BEGIN{exclude}
double approx_integral_mid_vectorized(FuncDouble2Double f, double p,
                                      double q, unsigned long n)
{
  double fsum = 0.0, d = (q - p)/n;
  const unsigned long units = 4, rest = n % units;
  unsigned long idx;
  __m256d v_fsum = _mm256_setzero_pd();

  assert(n > 0 && p < q);
  if (n >= units)
  {
    double *out, start = p + d/2;
    for (idx = 0; idx <= n - units; idx += units)
    {
      __m256d current = _mm256_set_pd(f(start),
                                      f(start + d),
                                      f(start + d + d),
                                      f(start + 3 * d));
      v_fsum = _mm256_add_pd(v_fsum,current);
      start += (units * d);
    }
    out = (double *) &v_fsum;
    for (idx = 0; idx < units; idx++)
    {
      fsum += out[idx];
    }
  }
  for (idx = 0; idx < rest; idx++)
  {
    assert(n - 1 >= idx);
    fsum += f(p + (n - 1 - idx) * d + d/2);
  }
  return d * fsum;
}
//END{exclude}

typedef double (*NumericIntegrationMethod)(FuncDouble2Double,
                                           double,double,
                                           unsigned long);

void approx_integral_generic_print(NumericIntegrationMethod imethod,
                                   FuncDouble2Double f,
                                   double p, double q,
                                   unsigned long maxsteps,
                                   double expected)
{
  unsigned long n;

  for (n = 10; n <= maxsteps; n *= 10)
  {
    double numerical = imethod(f,p,q,n);
    printf("n = %7lu, numerical = %.6f",n,numerical);
    if (expected != DBL_MAX)
    {
      double diff = expected - numerical;
      printf(", diff=%.2e",diff);
    }
    printf("\n");
  }
}

//BEGIN{exclude}
#define START_TIMER time0 = get_usecs()
#define ELAPSED_TIME(TAG) time1 = get_usecs();\
                          printf("%s %lu us\n",TAG,time1 - time0)
//END{exclude}

int main(int argc,char *argv[])
{
  long maxsteps;
  double p, q, expected;
//BEGIN{exclude}
  unsigned long time0, time1;
//END{exclude}

  if (argc != 2 || sscanf(argv[1],"%ld",&maxsteps) != 1 || maxsteps < 1)
  {
    fprintf(stderr,"Usage: %s <maxsteps>\n",argv[0]);
    exit(EXIT_SUCCESS);
  }
  approx_integral_trpz_print(curvedM,"curvedM",0.0,4.0,
                             maxsteps,DBL_MAX);
  expected = velocity_antiderivative(1.0) -
             velocity_antiderivative(0.0);
  p = 0;
  q = 1.0;
  approx_integral_trpz_print(velocity,"velocity",0.0,1.0,
                             maxsteps,expected);
  printf("method: approx_integral_trpz, integral(velocity,%.1f,%.1f):\n",p,q);
  approx_integral_generic_print(approx_integral_trpz,velocity,p,q,
                                maxsteps,expected);
  printf("%*.*sexpected = %.6f\n",14,14,"",expected);
//BEGIN{exclude}
  START_TIMER;
//END{exclude}
  printf("method: approx_integral_mid, integral "
         "(velocity,%.1f,%.1f):\n",p,q);
  approx_integral_generic_print(approx_integral_mid,velocity,
                                p,q, maxsteps,expected);
  printf("%*.*sexpected = %.6f\n",14,14,"",expected);
//BEGIN{exclude}
  ELAPSED_TIME("approx_integral_mid");
  START_TIMER;
  printf("method: approx_integral_mid_vectorized, integral "
         "(velocity,%.1f,%.1f):\n",p,q);
  approx_integral_generic_print(approx_integral_mid_vectorized,velocity,
                                p,q, maxsteps,expected);
  printf("%*.*sexpected = %.6f\n",14,14,"",expected);
  ELAPSED_TIME("approx_integral_mid_vectorized");
//END{exclude}
  exit(EXIT_SUCCESS);
}
