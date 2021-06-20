#include "array2dim.h"
void array2dim_example(unsigned long m,unsigned long n)
{
  unsigned long row, column;
  double **a2dim;

  /* create a m x n double array */
  array2dim_malloc(a2dim,double,m,n);

  /* use a2dim in conventional way via a2dim[row][column] */
  for (row = 0; row < m; row++) {
    for (column = 0; column < n; column++)
      a2dim[row][column] = 3.14 * row * column;
  }
  array2dim_delete(a2dim);
}

int main()
{
  array2dim_example(13,12);
  return EXIT_SUCCESS;
}
