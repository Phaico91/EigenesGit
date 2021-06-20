#include <stdlib.h>

/* Allocates a new 2-dimensional array of base type
   int with dimensions <rows> x <cols> and returns a
   pointer to the newly allocated space. */
int **intarray2dim_malloc(unsigned long rows,
                          unsigned long cols)
{
  unsigned long idx;
  int **a2dim = malloc(rows * sizeof *a2dim); /* row pointers */
  a2dim[0] = malloc(rows * cols * sizeof **a2dim); /* values */
  for (idx = 1; idx < rows; idx++)
    a2dim[idx] = a2dim[idx-1] + cols;
  return a2dim;
}

void intarray2dim_delete(int **a2dim)
{
  if (a2dim != NULL)
  {
    free(a2dim[0]); /* free array of values */
    free(a2dim);    /* free array of row pointers */
  }
}

void intarray2dim_example(unsigned long m,unsigned long n)
{
  unsigned long r, c;
  int **a2dim;

  /* create m x n int array */
  a2dim = intarray2dim_malloc(m, n);

  /* use a2dim in conventional way via a2dim[r][c] */
  for (r = 0; r < m; r++) {
    for (c = 0; c < n; c++)
      a2dim[r][c] = r * c;
  }
  intarray2dim_delete(a2dim);
}

int main()
{
  intarray2dim_example(13,12);
  return EXIT_SUCCESS;
}
