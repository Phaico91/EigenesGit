#include <stdio.h>
#include <stdlib.h>

int *copy_array(const int *arr,unsigned long len)
{
  unsigned long idx;
  int *copy = malloc(len * sizeof *copy);

  if (copy == NULL)
  {
    fprintf(stderr,"malloc(%lu) failed\n",len * sizeof *copy);
    exit(EXIT_FAILURE);
  }
  for (idx = 0; idx < len; idx++)
    copy[idx] = arr[idx];
  return copy;
}

int main(void)
{
  int arr1[] = {1,2,3,4};
  int *arr2 = copy_array(arr1,sizeof arr1/sizeof arr1[0]);
  free(arr2);
  return EXIT_SUCCESS;
}
