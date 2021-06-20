#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

static void check_order(const unsigned long *array_sorted,
                        const unsigned long *orig_sorted,
                        unsigned long len)
{
  unsigned long idx;

  for (idx = 0; idx < len; idx++)
  {
    assert(idx == 0 || array_sorted[idx-1] <= array_sorted[idx]);
    if (array_sorted[idx] != orig_sorted[idx])
    {
      fprintf(stderr,"array_sorted[%lu] = %lu != %lu = orig_sorted[%lu]\n",
              (unsigned long) idx,(unsigned long) array_sorted[idx],
              (unsigned long) orig_sorted[idx],(unsigned long) idx);
      exit(EXIT_FAILURE);
    }
  }
}

static int compare_ulong(const void *va,const void *vb)
{
  const unsigned long a = *((const unsigned long *) va);
  const unsigned long b = *((const unsigned long *) vb);

  if (a < b)
  {
    return -1;
  }
  if (a > b)
  {
    return 1;
  }
  return 0;
}

void showrange(const unsigned long *a,unsigned long start,
               unsigned long end)
{
  unsigned long idx;

  for (idx = start; idx <= end; idx++)
  {
    printf("%lu ",a[idx]);
  }
  printf("\n");
}

static void fill_random(unsigned long *orig,unsigned long numofvalues)
{
  unsigned long idx;

  for (idx = 0; idx < numofvalues; idx++)
  {
    orig[idx] = drand48() * ULONG_MAX;
  }
}

void verify_sorter(void (*sorter)(unsigned long *a,
                                  unsigned long len),
                   unsigned long runs,
                   unsigned long numofvalues)
{
  unsigned long *array, *orig, r;

  srand48(366292341);
  array = malloc(numofvalues * sizeof *array);
  orig = malloc(numofvalues * sizeof *orig);
  for (r = 0; r < (unsigned long) runs; r++)
  {
    fill_random(orig,numofvalues);
    memcpy(array,orig,numofvalues * sizeof *orig);
    sorter(array,numofvalues);
    qsort(orig,(size_t) numofvalues,sizeof *orig,compare_ulong);
    check_order(array,orig,numofvalues);
  }
  free(array);
  free(orig);
}
