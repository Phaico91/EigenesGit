#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
  int idx;
  const int rows = 10;
  const int cols = 2;
  int matrix[rows][cols];
  printf("sizeof matrix=%lu=rows*cols*sizeof (int) "
         " = %d * %d * %lu\n",
          sizeof matrix,rows,cols,sizeof (int));
  assert(sizeof matrix == rows * cols * sizeof (int));
  for (idx = 0; idx < rows; idx++)
  {
    printf("matrix[%d]=%p\n",idx,matrix[idx]);
  }
  exit(EXIT_SUCCESS);
}
