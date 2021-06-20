#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>

unsigned long *integer_dist(const unsigned long *arr,
                            size_t len,unsigned long maxval) {
  const unsigned long *ptr;
  unsigned long *dist = calloc(maxval + 1,sizeof *dist);
  assert(arr != NULL && dist != NULL);
  for (ptr = arr; ptr < arr + len; ptr++) {
    assert(*ptr <= maxval);
    dist[*ptr]++;
  }
  return dist;
}

int main(void)
{
  unsigned long arr[] = {1,2,1,3,4,2,2,1,1,0,1},
                maxval = ULONG_MAX,
                *ptr;
  const size_t len = sizeof arr/sizeof arr[0];
  for (ptr = arr; ptr < arr + len; ptr++)
  {
    if (maxval == ULONG_MAX || maxval < *ptr)
    {
      maxval = *ptr;
    }
  }
  if (maxval != ULONG_MAX)
  {
    unsigned long idx,
       *dist = integer_dist(arr,len,maxval);
    for (idx = 0; idx <= maxval; idx++)
    {
      printf("%lu\t%lu\n",idx,dist[idx]);
    }
    free(dist);
  }
  return EXIT_SUCCESS;
}
