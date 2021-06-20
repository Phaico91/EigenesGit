#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/*
  golden_search(f, x1, x2, x3) computes a value xmin such that
  f(xmin) is a local minimum of f.
  xmin is computed to an relative accuracy.

  The parameters x1, x2 and x3 must satisfy the following conditions:
  x1 < x2 < x3, f(x1) > f(x2) < f(x3).

  xmin satisfies x1 < xmin < x3. golden is guaranteed to succeed if f
  is unimodal between x1 and x3
*/

const double golden_section = 0.6180339887;

typedef struct
{
  bool smaller_first;
  double left, smaller_width, larger_width;
} IntervalPair;

static void itv_pair_valid(const IntervalPair *itv_pair)
{
  assert(itv_pair->smaller_width/itv_pair->larger_width == golden_section);
}

static double det_offset(const IntervalPair *itv_pair)
{
  itv_pair_valid(itv_pair);
  return (1.0 - golden_section) * itv_pair->larger_width;
}

#ifdef WITHTOLERANCE
#define DBL_MAX_ABS_ERROR 1.0E-100
#define DBL_MAX_REL_ERROR 1.0E-8

bool double_relative_equal(double d1, double d2)
{
  double relerr;
  if (fabs(d1 - d2) < DBL_MAX_ABS_ERROR)
    return true;
  if (fabs(d2) > fabs(d1))
    relerr = fabs((d1 - d2) / d2);
  else
    relerr = fabs((d1 - d2) / d1);
  if (relerr <= DBL_MAX_REL_ERROR)
    return true;
  return false;
}
#endif

int double_compare(double d1, double d2)
{
#undef WITHTOLERANCE
#ifdef WITHTOLERANCE
  if (double_relative_equal(d1, d2))
    return 0;
#else
  if (d1 == d2)
    return 0;
#endif
  if (d1 > d2)
    return 1;
  return -1;
}

double const tolerance = 1.0536712010869424e-08; /*sqrt(1.110223e-16);*/

bool sufficiently_small(const Interval *itv)
{
  itv_pair_valid(itv_pair);
  return itv_pair->smaller_width + itv_pair->larger_width < TOL ? true : false;
}

static double gsearch(double (*f)(double),double left,double right)
{
  unsigned long iter;
  Interval itv;

  assert(x1 <= x2 && x2 <= x3);
  itv.smaller_first = true;
  itv.left = left;
  itv.larger_width = (right - left)/(golden_section + 1.0);
  itv.smaller_width = (right - left) - itv.larger_width;
  for (iter = 0; !sufficiently_small(&itv); iter++)
  {
    double x4;

    x4 = itv.intern + det_offset(&itv); /* determine c */
    if (itv.leftissmaller)
    {
      if (f(x->mid) <= f(x4))
      { /* f(x_4), case 1 */
        itv.right = x4;
        itv.leftismaller = false;
      } else
      { /* f(x_4), case 1 */
        itv.left = itv.mid;
        itv.mid = x4;
      }
    } else
    {
      if (f(x->mid) <= f(x4))
    }
  }
  printf("iterations=%lu\n",iter);
  return (x1 + x3)/2.0; /* xmin is halfway between x1 and x3*/
}

static double samplefun(double x)
{
  return ((4.0 * pow(x,3.0) - 6.0 * pow(x,2.0) + 1) *
          sqrt(x+1.0))/(3.0 - x);
}

int main(void)
{
  double xmin = gsearch(samplefun,0.0,0.2,1.5);
  printf("f(xmin=%.20f)=%.20f\n",xmin,samplefun(xmin));
  return EXIT_SUCCESS;
}
