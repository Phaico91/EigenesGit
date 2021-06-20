#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static unsigned long find_index_of_smallest(
                       const unsigned long *array,
                       unsigned long from,
                       unsigned long numofelems)
{
  unsigned long j, index_of_smallest = from;

  for (j = from+1; j < numofelems; j++)
    if (array[j] < array[index_of_smallest])
      index_of_smallest = j;
  return index_of_smallest;
}

void selectionsort (unsigned long *array,
                    unsigned long numofelems)
{
  unsigned long i;

  assert (numofelems > 0);
  for (i = 0; i < numofelems - 1; i++) {
    unsigned long index_of_smallest
      = find_index_of_smallest(array,i,numofelems);

    assert(i <= index_of_smallest);
    if (i < index_of_smallest)
    { /* exchange elements at these indices */
      unsigned long tmp = array[index_of_smallest];
      array[index_of_smallest] = array[i];
      array[i] = tmp;
    }
  }
}

int main (void)
{
  unsigned long test_array[] = { 5, 2, 7, 9, 1,
                                 4, 3, 8, 6 },
                numofelems = sizeof test_array/
                             sizeof test_array[0],
                idx;

  selectionsort (test_array, numofelems);
  for (idx = 0; idx < numofelems; idx++)
  {
    printf ("%lu ", test_array[idx]);
    assert (idx == 0 || test_array[idx - 1] <= test_array[idx]);
  }
  printf ("\n");
  return EXIT_SUCCESS;
}
