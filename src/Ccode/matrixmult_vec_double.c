/* code generated by ./matrixmult_codegen.py 256 double */
static void multiply_1overhang(Matrix *C, const Matrix *A, const Matrix *B,
                               size_t start_j)
{
  for (size_t i = 0; i < C->rows; i++)
  {
    __m256d r = _mm256_setzero_pd();
    for (size_t k = 0; k < A->columns; k++)
    {
      __m256d a = _mm256_set1_pd(MA(i, k)),
              b = _mm256_set_pd(MB(k, start_j),
                                0,
                                0,
                                0),
              s = _mm256_mul_pd(a, b);
      r = _mm256_add_pd(r, s);
    }
double*     out = (double*) &r;
    MC(i, start_j) += out[3];
  }
}

static void multiply_2overhang(Matrix *C, const Matrix *A, const Matrix *B,
                               size_t start_j)
{
  for (size_t i = 0; i < C->rows; i++)
  {
    __m256d r = _mm256_setzero_pd();
    for (size_t k = 0; k < A->columns; k++)
    {
      __m256d a = _mm256_set1_pd(MA(i, k)),
              b = _mm256_set_pd(MB(k, start_j),
                                MB(k, start_j + 1),
                                0,
                                0),
              s = _mm256_mul_pd(a, b);
      r = _mm256_add_pd(r, s);
    }
double*     out = (double*) &r;
    MC(i, start_j) += out[3];
    MC(i, start_j + 1) += out[2];
  }
}

static void multiply_3overhang(Matrix *C, const Matrix *A, const Matrix *B,
                               size_t start_j)
{
  for (size_t i = 0; i < C->rows; i++)
  {
    __m256d r = _mm256_setzero_pd();
    for (size_t k = 0; k < A->columns; k++)
    {
      __m256d a = _mm256_set1_pd(MA(i, k)),
              b = _mm256_set_pd(MB(k, start_j),
                                MB(k, start_j + 1),
                                MB(k, start_j + 2),
                                0),
              s = _mm256_mul_pd(a, b);
      r = _mm256_add_pd(r, s);
    }
double*     out = (double*) &r;
    MC(i, start_j) += out[3];
    MC(i, start_j + 1) += out[2];
    MC(i, start_j + 2) += out[1];
  }
}


void matrix_multiplication_vectorized(Matrix *C,const Matrix *A,const Matrix *B)
{
  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);

  /* square parts of matrix */
  if (C->columns > 3)
  {
    for (size_t i = 0; i < C->rows; i++)
    {
      for (size_t j = 0; j < C->columns - 3; j += 4)
      {
        __m256d r = _mm256_setzero_pd();

        for (size_t k = 0; k < A->columns; k++)
        {
          __m256d a = _mm256_set1_pd(MA(i, k)),
                  b = _mm256_set_pd(MB(k, j),
                                      MB(k, j + 1),
                                      MB(k, j + 2),
                                      MB(k, j + 3)),
                  s = _mm256_mul_pd(a, b);
          r = _mm256_add_pd(r, s);
        }
    double*     out = (double*) &r;
        MC(i, j) = out[3];
        MC(i, j + 1) += out[2];
        MC(i, j + 2) += out[1];
        MC(i, j + 3) += out[0];
      }
    }
  }

  if (C->columns % 4 != 0)
  {
    const size_t ohang = C->columns - (C->columns % 4);
    switch (C->columns % 4)
    {
      case 1:
        multiply_1overhang(C, A, B, ohang);
        break;
      case 2:
        multiply_2overhang(C, A, B, ohang);
        break;
      case 3:
        multiply_3overhang(C, A, B, ohang);
        break;
      default: assert (false); //something went wrong
    }
  }
}

void matrix_multiplication_transposed_vectorized(Matrix *C,const Matrix *A,
                                                 const Matrix *B)
{
  Matrix *M;

  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);

  M = matrix_transposition(B);

  /* square parts of matrix */
  if (C->columns > 3)
  {
    for (size_t i = 0; i < C->rows; i++)
    {
      for (size_t j = 0; j < C->columns - 3; j += 4)
      {
        __m256d r = _mm256_setzero_pd();

        for (size_t k = 0; k < A->columns; k++)
        {
          __m256d a = _mm256_set1_pd(MA(i, k)),
                  b = _mm256_set_pd(MM(j, k),
                                      MM(j + 1, k),
                                      MM(j + 2, k),
                                      MM(j + 3, k)),
                  s = _mm256_mul_pd(a, b);
          r = _mm256_add_pd(r, s);
        }
    double*     out = (double*) &r;
        MC(i, j) = out[3];
        MC(i, j + 1) += out[2];
        MC(i, j + 2) += out[1];
        MC(i, j + 3) += out[0];
      }
    }
  }

  if (C->columns % 4 != 0)
  {
    const size_t ohang = C->columns - (C->columns % 4);
    switch (C->columns % 4)
    {
      case 1:
        multiply_1overhang(C, A, B, ohang);
        break;
      case 2:
        multiply_2overhang(C, A, B, ohang);
        break;
      case 3:
        multiply_3overhang(C, A, B, ohang);
        break;
      default: assert (false); //something went wrong
    }
  }
  matrix_delete(M);
}