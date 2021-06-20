#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>  /* necessary to include */
double product(int number, ...)
{
  va_list list;
  double p = 1.0;
  int i;

  va_start(list, number); /* arg to start with */
  for (i = 0; i < number; i++)
    p *= va_arg(list, double); /* access double parameters
                                  following number */
  va_end(list); /* no further args */
  return p;
}

int main(void)
{
  printf("%.2f\n",product(3,3.14,2.5,7.5));
  return EXIT_SUCCESS;
}
