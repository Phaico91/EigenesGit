#include <stdio.h>
#include <stdlib.h>
double average_double(const double *arr,
                      unsigned long len)
{
  const double *ptr;
  double sum = 0.0;

  for (ptr = arr; ptr < arr + len; ptr++)
  {
    sum += *ptr;
  }
  return sum/len;
}

int main(void)
{
  const double ds[] = {3.14,5.27,1.0,2,87};
  printf("avg=%.2f\n",average_double(ds,sizeof ds/sizeof ds[0]));
  return EXIT_SUCCESS;
}
