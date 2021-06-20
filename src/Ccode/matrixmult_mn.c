#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "matrixmult_opt.h"
#include "matrixmult.h"

static void cross_compare(const Matrix * const *Ctab)
{
  unsigned int i = 0;
  const unsigned int last_op = matrix_mult_options_last_op();

  while (i < last_op)
  {
    if (Ctab[i] != NULL)
    {
      unsigned int j = i+1;

      while (j <= last_op)
      {
        if (Ctab[j] != NULL)
        {
          matrix_compare(Ctab[i],Ctab[j]);
          break;
        }
        j++;
      }
      i = j;
    } else
    {
      i++;
    }
  }
}

static void matrixtab_delete(Matrix **Ctab)
{
  unsigned int i;
  const unsigned int last_op = matrix_mult_options_last_op();

  for (i = 0; i <= last_op; i++)
  {
    if (Ctab[i] != NULL)
    {
      matrix_delete(Ctab[i]);
    }
  }
  free(Ctab);
}

#define SHOW_RESULT_MATRIX(MATRIX)\
        if (matrix_mult_options_show(options))\
        {\
          matrix_show(MATRIX);\
        }

int main(int argc, char *argv[])
{
  Matrix *A, *B = NULL, **Ctab;
  MMoptions *options;
  FILE *fp = NULL;
  size_t rowsB = 0, columnsB = 0, rowsC = 0, columnsC = 0;
  const char *inputfileA, *inputfileB;
  const unsigned int last_op = matrix_mult_options_last_op();

  options = matrix_mult_options_new(argc,argv);
  if (matrix_mult_options_showmatrixbasetype(options))
  {
    printf("%s\n",Matrixbasetype_string());
    matrix_mult_options_delete(options);
    return EXIT_SUCCESS;
  }
  matrix_mult_options_show_options(options);
  inputfileA = matrix_mult_options_inputfileA(options);
  if (inputfileA != NULL)
  {
    A = matrix_new_read(inputfileA);
    if (A == NULL)
    {
      fprintf(stderr,"%s: cannot open file %s\n",argv[0],inputfileA);
      matrix_mult_options_delete(options);
      return EXIT_FAILURE;
    }
  } else
  {
    if (matrix_mult_options_continuous(options))
    {
      A = matrix_new_continuous(matrix_mult_options_rowsA(options),
                                matrix_mult_options_columnsA(options));
    } else
    {
      A = matrix_new_random(matrix_mult_options_rowsA(options),
                            matrix_mult_options_columnsA(options));
    }
  }
  if (matrix_mult_options_show(options))
  {
    matrix_show(A);
  }
  inputfileB = matrix_mult_options_inputfileB(options);
  /* for splitting we only need matrix A. So we generate the other matrices
     only if any other operation is specified */
  if (!matrix_mult_options_only_split(options))
  {
    if (inputfileB != NULL)
    {
      rowsC = matrix_rows(A);
      B = matrix_new_read(inputfileB);
      if (B == NULL)
      {
        fprintf(stderr,"%s: cannot open file %s\n",argv[0],inputfileA);
        matrix_mult_options_delete(options);
        matrix_delete(A);
        return EXIT_FAILURE;
      }
      rowsB = matrix_rows(B);
      columnsC = columnsB = matrix_columns(B);
    } else
    {
      rowsB = matrix_mult_options_columnsA(options);
      columnsC = columnsB = matrix_mult_options_columnsB(options);
      rowsC = matrix_mult_options_rowsA(options);
      if (matrix_mult_options_continuous(options))
      {
        B = matrix_new_continuous(rowsB,columnsB);
      } else
      {
        B = matrix_new_random(rowsB,columnsB);
      }
    }
  }
  if (matrix_mult_options_show(options))
  {
    matrix_show(B);
  }
  if (matrix_mult_options_split(options))
  {
    Matrix *Aupper = matrix_h_split_upper(A),
	   *Alower = matrix_h_split_lower(A),
	   *Aleft = matrix_v_split_left(A),
	   *Aright = matrix_v_split_right(A);

    if (matrix_mult_options_show(options))
    {
      matrix_show(Aupper);
      matrix_show(Alower);
      matrix_show(Aleft);
      matrix_show(Aright);
    }
    matrix_delete(Aupper);
    matrix_delete(Alower);
    matrix_delete(Aleft);
    matrix_delete(Aright);
  }
  if (matrix_mult_options_addition(options))
  {
    Matrix *C;
    if (matrix_rows(A) != rowsB || matrix_columns(A) != columnsB)
    {
      fprintf(stderr,"matrix addition requires that matrices have same "
		     "number of rows and columns\n");
      exit(EXIT_FAILURE);
    }
    C = matrix_new(rowsB,columnsB);
    matrix_addition(C,A,B);
    SHOW_RESULT_MATRIX(C);
    matrix_delete(C);
  }
  Ctab = (Matrix **) calloc(last_op + 1,sizeof *Ctab);
  if (matrix_mult_options_multiplication_standard(options))
  {
    Matrix *C = Ctab[OP_multiplication_standard] = matrix_new(rowsC,columnsC);
    matrix_multiplication(C,A,B);
    SHOW_RESULT_MATRIX(C);
  }
  if (matrix_mult_options_multiplication_transposed(options))
  {
    Matrix *C = Ctab[OP_multiplication_transposed] = matrix_new(rowsC,columnsC);
    matrix_multiplication_transposed(C,A,B);
    SHOW_RESULT_MATRIX(C);
  }
  if (matrix_mult_options_multiplication_dc(options))
  {
    Matrix *C = Ctab[OP_multiplication_dc] = matrix_new(rowsC,columnsC);
    const size_t dc_width_threshold
      = matrix_mult_options_dc_width_threshold(options);
    if (matrix_mult_options_num_threads(options) == 2)
    {
      matrix_multiplication_dc_2threads(0,C,A,B,dc_width_threshold);
    } else
    {
      assert(matrix_mult_options_num_threads(options) == 1);
      matrix_multiplication_dc_iter(C,A,B,dc_width_threshold);
      /*printf("# recursive_calls: %lu\n",recursive_calls_dc());
      printf("# directmultiplications: %lu\n",directmultiplications_dc());*/
    }
    SHOW_RESULT_MATRIX(C);
  }
  if (matrix_mult_options_multiplication_vector(options))
  {
    Matrix *C = Ctab[OP_multiplication_vector] = matrix_new(rowsC,columnsC);
    matrix_multiplication_vectorized(C,A,B);
    SHOW_RESULT_MATRIX(C);
  }
  if (matrix_mult_options_multiplication_transposed_vector(options))
  {
    Matrix *C = Ctab[OP_multiplication_transposed_vector]
      = matrix_new(rowsC,columnsC);
    matrix_multiplication_transposed_vectorized(C,A,B);
    SHOW_RESULT_MATRIX(C);
  }
  matrix_delete(B);
  cross_compare((const Matrix * const *) Ctab);
  matrixtab_delete(Ctab);
  if (fp != NULL)
  {
    fclose(fp);
  }
  matrix_delete(A);
  matrix_mult_options_delete(options);
  return EXIT_SUCCESS;
}
