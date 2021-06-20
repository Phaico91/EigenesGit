#include <stdio.h>
#include <stdlib.h>
#include <float.h>

float accuracy_float(void) {
  float f, eps = (float) 1.0;
  for (f = (float) 1.0;
       (float) 1.0 + f >
       (float) 1.0;
       f /= (float) 2.0) eps = f;
  return eps;
}

double accuracy_double(void) {
  double f, eps = (double) 1.0;
  for (f = (double) 1.0;
       (double) 1.0 + f >
       (double) 1.0;
       f /= (double) 2.0) eps = f;
  return eps;
}

long double accuracy_long_double(void) {
  long double f, eps = (long double) 1.0;
  for (f = (long double) 1.0;
       (long double) 1.0 + f >
       (long double) 1.0;
       f /= (long double) 2.0) eps = f;
  return eps;
}

int main(void)
{
  float f = accuracy_float();
  double d = accuracy_double();
  long double ld
    = accuracy_long_double();
  printf("accuracy float=%.6e\n",f);
  printf("accuracy double=%.6e\n",d);
  printf("accuracy long double=%.6Le\n",ld);
  return EXIT_SUCCESS;
}
