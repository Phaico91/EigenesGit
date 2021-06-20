#include <stdlib.h>
#include "array2dim.h"

void applyscalar_via_switch(double **matrix,double scalar,
                            unsigned long rows,
                            unsigned long cols,char opCode)
{
  unsigned long r, c;

  for (r = 0; r < rows; r++)
    for (c = 0; c < cols; c++)
      switch (opCode)
      {
        case '+' : matrix[r][c] += scalar; break;
        case '-' : matrix[r][c] -= scalar; break;
        case '*' : matrix[r][c] *= scalar; break;
      }
}

typedef double (*Scalarfunc)(double,double);
void applyscalar_via_funptr(double **matrix,double scalar,
                            unsigned long rows,
                            unsigned long cols,
                            Scalarfunc op_func)
{
  unsigned long r, c;

  for (r = 0; r < rows; r++)
    for (c = 0; c < cols; c++)
      matrix[r][c] = op_func(matrix[r][c], scalar);
}

double plus    (double a, double b) { return a+b; }
double minus   (double a, double b) { return a-b; }
double multiply(double a, double b) { return a*b; }

int main(void)
{
  unsigned long r, c;
  const unsigned long rows = 1000, cols = 500;
  double **matrix;

  array2dim_malloc(matrix, double, rows, cols);
  for (r = 0; r < rows; r++)
    for (c = 0; c < cols; c++)
      matrix[r][c] = drand48(); /* random val in [0,1) */
  applyscalar_via_switch(matrix,3.14,rows,cols,'+');
  applyscalar_via_funptr(matrix,3.14,rows,cols,minus);
  applyscalar_via_funptr(matrix,3.14,rows,cols,multiply);
  array2dim_delete(matrix);
  return EXIT_SUCCESS;
}
