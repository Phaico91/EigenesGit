#include <stdlib.h>

int **int_matrix_new(unsigned long rows, 
                     unsigned long cols)
{
  unsigned long idx; 
  /* row pointers: */
  int **matrix = (int **) malloc(rows * sizeof *matrix); 
  /* values: */
  matrix[0] = (int *) malloc(rows * cols * sizeof **matrix); 
  for (idx = 1; idx < rows; idx++) 
    matrix[idx] = matrix[idx-1] + cols; 
  return matrix;
}

void int_matrix_delete(int **matrix) 
{
  if (matrix != NULL)
  {
    free(matrix[0]); /* free array of values */
    free(matrix);    /* free array of row pointers */
  }
}

void int_matrix_example(unsigned long m,unsigned long n)
{
  unsigned long r, c;
  int **matrix;

  /* create m x n int array */
  matrix = int_matrix_new(m, n);

  /* use matrix in conventional way via matrix[r][c] */
  for (r = 0; r < m; r++) {
    for (c = 0; c < n; c++)
      matrix[r][c] = r * c;
  }
  int_matrix_delete(matrix);
}

int main(void)
{
  int_matrix_example(13,12);
  return EXIT_SUCCESS;
}
