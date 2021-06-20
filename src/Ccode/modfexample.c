#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
  double d, ip, f;

  d = 4.74234;
  f = modf(d,&ip);
  printf("f=%.4f,ip=%.4f\n",f,ip);
  return EXIT_SUCCESS;
}
