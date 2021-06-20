#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int double_cmp(const void *aptr, const void *bptr)
{
  double a = *((const double *) aptr);
  double b = *((const double *) bptr);

  return (a < b) ? -1
                 : ((a > b) ? 1 : 0);
}

int main(int argc,char *argv[])
{
  int idx, len;
  double *arr;
  if (argc != 2 || sscanf(argv[1],"%d",&len) != 1 || len <= 0)
  {
    fprintf(stderr,"Usage: %s <positive number>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  arr = malloc(len * sizeof *arr);
  assert(arr != NULL);
  srand48(366292341); /* init random number generator */
  for (idx =0;idx<len;idx++)
    arr[idx] = drand48();
  qsort(arr, (size_t) len, sizeof *arr, double_cmp);
  for (idx=1;idx<len;idx++) /* check for correct order */
    assert(arr[idx-1] <= arr[idx]);
  free(arr);
  exit(EXIT_SUCCESS);
}
