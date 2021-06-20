#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "verify_sorter.h"

static unsigned long distribute_elements(
     unsigned long *buf, /* intermediate buffer */
     unsigned long *a,  /* array of elements to distribute */
     unsigned long len) /* number of elements in array */
{
  unsigned long idx, sidx = 0, lidx = len - 1,
                pivot = a[len-1]; /* use last element */

  for (idx = 0; idx < len - 1; idx++)
  {
    assert(sidx < lidx);
    if (a[idx] < pivot)
      buf[sidx++] = a[idx]; /* to smaller part, left to right */
    else
      buf[lidx--] = a[idx]; /* to larger part, right to left */
  }
  for (idx = 0; idx < len; idx++)
    a[idx] = buf[idx]; /* copy back buffered elements */
  assert(sidx == lidx);
  a[lidx] = pivot; /* put pivot between smaller and larger part */
  return lidx; /* return index where pivot is stored */
}

void quicksort(unsigned long *a,
               unsigned long *buf,
               unsigned long len)
{
  if (len > 1)
  {
    unsigned long pivotidx
      = distribute_elements(buf,a,len);
    /* sort part left of pivot */
    quicksort(a,buf,pivotidx);
    /* sort part right of pivot */
    quicksort(a+pivotidx+1,
              buf+pivotidx+1,
              len - (pivotidx + 1));
  }
}

void quicksort_all(unsigned long *a,
                   unsigned long len)
{
  unsigned long *buf = malloc(len * sizeof *buf);
  quicksort(a,buf,len);
  free(buf);
}

int main(int argc,char *argv[])
{
  long runs, numofvalues_read;

  if (argc != 3 || sscanf(argv[1],"%ld",&runs) != 1 || runs < 1L ||
      sscanf(argv[2],"%ld",&numofvalues_read) != 1 || numofvalues_read < 1L)
  {
    fprintf(stderr,"Usage: %s <runs> <numofvalues>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  verify_sorter(quicksort_all,(unsigned long) runs,
                (unsigned long) numofvalues_read);
  return EXIT_SUCCESS;
}
