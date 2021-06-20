#include <stdio.h>
#define VECSIZE 10

int main(void)
{
  int sum = 0, vec[VECSIZE], idx; /*init of vec follows*/
  for (idx=0; idx<VECSIZE; idx++)
  {
    vec[idx] = idx;
  }

  {
    for (idx = 0; idx < VECSIZE; idx++)
      sum += vec[idx];
    printf("sum of numbers from 0 to %d: %d\n",
           VECSIZE-1, sum);
  }
  {
    int *v, sum = 0;
    for (v = vec; v < vec + VECSIZE; v++)
    {
      sum += *v;
    }
    printf("sum of numbers from 0 to %d: %d\n",
           VECSIZE-1, sum);
  }
  return 0;
}
