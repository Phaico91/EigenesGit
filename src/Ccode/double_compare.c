#include <math.h>
#include <stdbool.h>
#include "double_compare.h"

#define WITHTOLERANCE
#ifdef WITHTOLERANCE
#define DBL_MAX_ABS_ERROR 1.0E-100
#define DBL_MAX_REL_ERROR 1.0E-8

static bool double_relative_equal(double d1, double d2)
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
