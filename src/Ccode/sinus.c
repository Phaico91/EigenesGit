#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(void)
{
  double x;

  for (x = -10.0; x <= 10; x += 0.001)
    printf("%.2f\t%.2f\n",x,sin(x));
  return EXIT_SUCCESS;
}
