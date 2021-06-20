#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <pthread.h>
#ifdef __cplusplus
#include "array2dim.hpp"
#else
#include "array2dim.h"
#endif
#include "linebuffer.h"
#include "matrixmult.h"
#include <immintrin.h>

#define EXPECTED_EQUALITY(V1,V2)\
  if ((V1) != (V2))\
  {\
    fprintf(stderr,"file %s, line %d, function %s:\n"\
                   "%s = %lu != %lu = %s "\
                   "not expected\n",\
                   __FILE__,__LINE__,__func__,#V1,V1,V2,#V2);\
    exit(EXIT_FAILURE);\
  }

/* use undef to compile integer based version */
#define DOUBLEBASETYPE
#ifdef DOUBLEBASETYPE
typedef double Matrixbasetype;  /* from matrixmult.c */
#define MATRIXBASETYPE_IN_FORMAT "lf"
#define MATRIXBASETYPE_OUT_FORMAT ".2f"
#define EQUAL_Matrixbasetype(X,Y) (fabs((X) - (Y)) < 0.001)
#else
typedef int Matrixbasetype;
#define MATRIXBASETYPE_IN_FORMAT "d"
#define MATRIXBASETYPE_OUT_FORMAT MATRIXBASETYPE_IN_FORMAT
#define EQUAL_Matrixbasetype(X,Y) ((X) == (Y))
#endif

size_t Matrixbasetype_sizeof(void)
{
  return sizeof(Matrixbasetype);
}

const char *Matrixbasetype_string(void)
{
#ifdef DOUBLEBASETYPE
  return "double";
#else
  return "int";
#endif
}

struct Matrix    /* from matrixmult.c */
{
  Matrixbasetype **a2dim;
  size_t rows,        /* number of rows */
         columns,     /* number of columns */
         rowstart,    /* first row of submatrix */
         columnstart, /* first column of submatrix */
         origcols;    /* refers to original matrix */
  bool hasownvalues;  /* matrix has own values */
};

void matrix_update(Matrix *matrix,size_t i, size_t j,
                   Matrixbasetype value)
{
  assert(matrix != NULL && matrix->a2dim != NULL && matrix->a2dim[0] != NULL
         && i < matrix->rows && j < matrix->columns);
  matrix->a2dim[i][j] = value;
}

#define MACC(M,I,J)\
        ((M)->a2dim[I][J])
#define MM(I,J)     MACC(M,I,J)
#define MA(I,J)     MACC(A,I,J)
#define MB(I,J)     MACC(B,I,J)
#define MC(I,J)     MACC(C,I,J)

Matrix *matrix_new(size_t rows,size_t columns)
{
  Matrix *matrix = (Matrix *) malloc(sizeof *matrix);

  assert(matrix != NULL);
  array2dim_malloc(matrix->a2dim,Matrixbasetype,rows,columns);
  matrix->hasownvalues = true;
  matrix->rows = rows;
  matrix->columns = matrix->origcols = columns;
  matrix->rowstart = matrix->columnstart = 0;
  return matrix;
}

Matrix *matrix_new_random(size_t rows,
                          size_t columns)
{
  Matrix *M = matrix_new(rows,columns);
  const Matrixbasetype minval = 50.0;
  const Matrixbasetype maxval = 100.0;

  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < columns; j++)
    {
      MM(i,j) = minval +
                drand48() * (maxval-minval);
      assert(minval <= MM(i,j) &&
             MM(i,j) <= maxval);
    }
  }
  return M;
}

Matrix *matrix_new_read(const char *filename)
{
  size_t rows = 0, columns = ULONG_MAX;
  Matrix *matrix;
  Linebuffer line = {NULL,0,0};
  Matrixbasetype *values_space = 0;
  size_t values_nextfree = 0, values_allocated = 0;
  FILE *fpin = fopen(filename,"rb");

  if (fpin == NULL)
  {
    return NULL;
  }
  while (linebuffer_next(fpin, &line) != EOF)
  {
    Matrixbasetype readvalue;
    size_t this_columns = 0;
    char *ptr = line.space;
    int offset;

    while (ptr < line.space + line.nextfree &&
           sscanf(ptr,"%" MATRIXBASETYPE_IN_FORMAT "%n", &readvalue,&offset)
           == 1)
    {
      if (values_nextfree >= values_allocated)
      {
        values_allocated += (values_allocated * 0.2) + 128;
        values_space = (Matrixbasetype *)realloc(values_space,values_allocated);
      }
      assert(values_nextfree < values_allocated);
      values_space[values_nextfree++] = readvalue;
      this_columns++;
      ptr += offset + 1;
    }
    rows++;
    if (columns == ULONG_MAX)
    {
      columns = this_columns;
    } else
    {
      assert (columns == this_columns);
    }
    line.nextfree = 0;
  }
  assert(rows * columns == values_nextfree);
  matrix = matrix_new(rows,columns);
  memcpy(matrix->a2dim[0],values_space,values_nextfree * sizeof *values_space);
  free(values_space);
  free(line.space);
  return matrix;
}

Matrix *matrix_new_continuous(size_t rows,size_t columns)
{
  Matrix *M = matrix_new(rows,columns);
  Matrixbasetype minvalue = (Matrixbasetype) 0;

  assert(M != NULL);
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < columns; j++)
    {
      MM(i,j) = minvalue++;
    }
  }
  return M;
}

void matrix_delete(Matrix *matrix)
{
  if (matrix != NULL)
  {
    if (matrix->hasownvalues)
    {
      array2dim_delete(matrix->a2dim);
    } else
    {
      free(matrix->a2dim); /* only free row vector */
    }
    free(matrix);
  }
}

size_t matrix_rows(const Matrix *matrix)
{
  assert(matrix != NULL);
  return matrix->rows;
}

size_t matrix_columns(const Matrix *matrix)
{
  assert(matrix != NULL);
  return matrix->columns;
}

void matrix_show(const Matrix *M)
{
  Matrixbasetype absminvalue, absmaxvalue, minvalue = 0, maxvalue = 0;
  bool minvalue_defined = false, maxvalue_defined = false;
  int width;

  printf("%lu X %lu\n",M->rows,M->columns);
  for (size_t i = 0; i < M->rows; i++)
  {
    for (size_t j = 0; j < M->columns; j++)
    {
      if (!minvalue_defined || MM(i,j) < minvalue)
      {
        minvalue_defined = true;
        minvalue = MM(i,j);
      }
      if (!maxvalue_defined || MM(i,j) > maxvalue)
      {
        maxvalue_defined = true;
        maxvalue = MM(i,j);
      }
    }
  }
  absmaxvalue = maxvalue >= 0 ? maxvalue : -maxvalue;
  absminvalue = minvalue >= 0 ? minvalue : -minvalue;
  if (absminvalue > absmaxvalue)
  {
    absmaxvalue = absminvalue;
  }
  if (absmaxvalue == 0)
  {
    width = 2;
  } else
  {
    width = (int) log10((double) absmaxvalue) + 2;
  }
#ifdef DOUBLEBASETYPE
  width += 3;
#endif
  if (minvalue < 0 || maxvalue < 0)
  {
    width++;
  }
  for (size_t i = 0; i < M->rows; i++)
  {
    for (size_t j = 0; j < M->columns; j++)
    {
      printf("%*" MATRIXBASETYPE_OUT_FORMAT,width,MM(i,j));
    }
    printf("\n");
  }
}

static Matrixbasetype **matrix_rowptr(size_t rows,
                                      size_t origcols,
                                      Matrixbasetype *firstsubrow) {
  Matrixbasetype **a2dim
    = (Matrixbasetype **) malloc(rows * sizeof *a2dim);
  assert(a2dim != NULL);
  a2dim[0] = firstsubrow; /* pointer to the first row of submatrix */
  for (size_t idx = 1; idx < rows; idx++)
    a2dim[idx] = a2dim[idx-1] + origcols; /* consecutive rows at */
  return a2dim;                           /* distance origcols */
}                                         /* in original matrix */

Matrix *matrix_h_split_upper(const Matrix *parent)
{
  Matrix *M = (Matrix *) malloc(sizeof *M);

  assert(M != NULL && parent != NULL &&
                      parent->rows > 1);
  M->hasownvalues = false;
  M->origcols = parent->origcols;
  M->rows = parent->rows/2 +
            (parent->rows % 2); /* ceil */
  M->columns = parent->columns;
  M->rowstart = parent->rowstart;
  M->columnstart = parent->columnstart;
  M->a2dim = matrix_rowptr(M->rows,
                           M->origcols,
         /* firstsubrow */ parent->a2dim[0]);
  return M;
}

Matrix *matrix_h_split_lower(const Matrix *parent)
{
  Matrix *M = (Matrix *) malloc(sizeof *M);

  assert(M != NULL && parent != NULL &&
                      parent->rows > 1);
  M->hasownvalues = false;
  M->origcols = parent->origcols;
  M->rows = parent->rows/2;
  M->columns = parent->columns;
  size_t firstrows = parent->rows - M->rows;
  M->rowstart = parent->rowstart + firstrows;
  M->columnstart = parent->columnstart;
  M->a2dim = matrix_rowptr(M->rows,
                           M->origcols,
          /*firstsubrow:*/ parent->a2dim[firstrows]);
  return M;
}

Matrix *matrix_v_split_left(const Matrix *parent)
{
  Matrix *M = (Matrix *) malloc(sizeof *M);

  assert(M != NULL && parent != NULL &&
                      parent->columns > 1);
  M->hasownvalues = false;
  M->origcols = parent->origcols;
  M->rows = parent->rows;
  M->columns = parent->columns/2 +
               (parent->columns % 2); /* ceil */
  M->rowstart = parent->rowstart;
  M->columnstart = parent->columnstart;
  M->a2dim = matrix_rowptr(M->rows,
                           M->origcols,
          /*firstsubrow:*/ parent->a2dim[0]);
  return M;
}

Matrix *matrix_v_split_right(const Matrix *parent)
{
  Matrix *M = (Matrix *) malloc(sizeof *M);

  assert(M != NULL && parent != NULL &&
                      parent->columns > 1);
  M->hasownvalues = false;
  M->origcols = parent->origcols;
  M->rows = parent->rows;
  M->columns = parent->columns/2;
  M->rowstart = parent->rowstart;
  size_t fstcols = parent->columns - M->columns;
  M->columnstart = parent->columnstart + fstcols;
  M->a2dim = matrix_rowptr(M->rows,
                           M->origcols,
          /*firstsubrow:*/ parent->a2dim[0] + fstcols);
  return M;
}

void matrix_multiplication(Matrix *C,
                           const Matrix *A,
                           const Matrix *B)
{
  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);
  for (size_t i = 0; i < C->rows; i++)
  {
    for (size_t j = 0; j < C->columns; j++)
    {
      Matrixbasetype sum = (Matrixbasetype) 0;

      for (size_t k = 0; k < A->columns; k++)
      {
        sum += MA(i,k) * MB(k,j);
      }
      MC(i,j) += sum; /* we always add value */
    }
  }
}

static Matrix *matrix_transposition(const Matrix *A)
{
  Matrix *B = matrix_new(A->columns,A->rows);

  for (size_t r = 0; r < A->rows; r++)
  {
    for (size_t c = 0; c < A->columns; c++)
    {
      MB(c,r) = MA(r,c);
    }
  }
  return B;
}

void matrix_multiplication_transposed(Matrix *C,
                                      const Matrix *A,
                                      const Matrix *B)
{
  Matrix *M;

  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);
  M = matrix_transposition(B);
  for (size_t i = 0; i < C->rows; i++)
  {
    for (size_t j = 0; j < C->columns; j++)
    {
      Matrixbasetype sum = (Matrixbasetype) 0;

      for (size_t k = 0; k < A->columns; k++)
      {
        sum += MA(i,k) * MM(j,k);
      }
      MC(i,j) += sum; /* we always add value */
    }
  }
  matrix_delete(M);
}

#ifdef DOUBLEBASETYPE
#include "matrixmult_vec_double.c"
#else
#include "matrixmult_vec_int.c"
#endif

void matrix_addition(Matrix *C,const Matrix *A,const Matrix *B)
{
  EXPECTED_EQUALITY(A->rows,B->rows);
  EXPECTED_EQUALITY(A->columns,B->columns);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,A->columns);
  for (size_t i = 0; i < C->rows; i++)
  {
    for (size_t j = 0; j < C->columns; j++)
    {
      MC(i,j) = MA(i,j) + MB(i,j);
    }
  }
}

void matrix_addition_inplace(Matrix *A,const Matrix *B)
{
  EXPECTED_EQUALITY(A->rows,B->rows);
  EXPECTED_EQUALITY(A->columns,B->columns);
  for (size_t i = 0; i < A->rows; i++)
  {
    for (size_t j = 0; j < A->columns; j++)
    {
      MA(i,j) += MB(i,j);
    }
  }
}

void matrix_compare(const Matrix *A,const Matrix *B)
{
  EXPECTED_EQUALITY(A->rows,B->rows);
  EXPECTED_EQUALITY(A->columns,B->columns);
  for (size_t i = 0; i < A->rows; i++)
  {
    for (size_t j = 0; j < A->columns; j++)
    {
      if (!EQUAL_Matrixbasetype(MA(i,j),MB(i,j)))
      {
        fprintf(stderr,"A[%lu][%lu] = %" MATRIXBASETYPE_OUT_FORMAT " != %"
                             MATRIXBASETYPE_OUT_FORMAT " = B[%lu[%lu]\n",
                        i,j,MA(i,j),MB(i,j),i,j);
        exit(EXIT_FAILURE);
      }
    }
  }
}

#undef DEBUG
#ifdef DEBUG
#define INDENT        printf("%*s",2*level,"")
static void showcase(int level,const char *c,const Matrix *A,
                     const Matrix *B,const Matrix *C,size_t maxwidth)
{
  char *rootprefix;
  INDENT;
  if (level == 0)
  {
    rootprefix = "\\";
  } else
  {
    rootprefix = "child { ";
  }
  if (strcmp(c,"1") == 0)
  {
    printf("child { node {\\ShowDCLeaf{%lu}{%lu}}}\n",A->rows,A->columns);
  } else
  {
    printf("%snode {\\ShowDCcoords{%s}{%lu}",rootprefix,c,maxwidth);
    printf("{%lu}{%lu}{%lu}{%lu}{%lu}{%lu}}\n",
           A->rows,A->columns,B->rows,B->columns,C->rows,C->columns);
  }
}
#define SHOWCASE(X)  showcase(level,X,A,B,C,maxwidth)

#define SHOWCASEEND  if (level > 0)\
                     {\
                       INDENT;\
                       printf("}\n");\
                     }
#else
static size_t directmultiplications = 0;
#define SHOWCASE(X)  if (strcmp(X,"1") == 0)\
                     {\
                       directmultiplications++;\
                     }
#define SHOWCASEEND  /* Nothing */
#endif
#define MAX2(X,Y) ((X) > (Y) ? (X) : (Y))

static size_t recursive_calls = 0;

void matrix_multiplication_dc(int level,Matrix *C,
                              const Matrix *A,const Matrix *B,
                              size_t widththreshold)
{
  recursive_calls++;
  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);
  size_t m = A->rows;
  size_t n = A->columns;
  size_t l = B->columns;
  size_t maxwidth = MAX2(m,MAX2(n,l));
  if (maxwidth <= widththreshold)
  {
    SHOWCASE("1"); /* output leaf of recursion tree */
    matrix_multiplication(C,A,B);
    return;
  }
  if (maxwidth == m)
  {
    Matrix *A1, *A2, *C1, *C2;

    SHOWCASE("2a"); /* output internal node of recursion tree */
    A1 = matrix_h_split_upper(A);
    A2 = matrix_h_split_lower(A);
    C1 = matrix_h_split_upper(C);
    C2 = matrix_h_split_lower(C);
    matrix_multiplication_dc(level+1,C1,A1,B,
                             widththreshold);
    matrix_multiplication_dc(level+1,C2,A2,B,
                             widththreshold);
    matrix_delete(A1);
    matrix_delete(A2);
    matrix_delete(C1);
    matrix_delete(C2);
    SHOWCASEEND;
    return;
  }
  if (maxwidth == l)
  {
    Matrix *B1, *B2, *C1, *C2;

    SHOWCASE("2b"); /* output internal node of recursion tree */
    B1 = matrix_v_split_left(B);
    B2 = matrix_v_split_right(B);
    C1 = matrix_v_split_left(C);
    C2 = matrix_v_split_right(C);
    matrix_multiplication_dc(level+1,C1,A,B1,
                             widththreshold);
    matrix_multiplication_dc(level+1,C2,A,B2,
                             widththreshold);
    matrix_delete(B1);
    matrix_delete(B2);
    matrix_delete(C1);
    matrix_delete(C2);
    SHOWCASEEND;
    return;
  }
  {
    Matrix *A1, *A2, *B1, *B2;

    assert (maxwidth == n);
    SHOWCASE("2c"); /* output internal node of recursion tree */
    A1 = matrix_v_split_left(A);
    A2 = matrix_v_split_right(A);
    B1 = matrix_h_split_upper(B);
    B2 = matrix_h_split_lower(B);
    matrix_multiplication_dc(level+1,C,A1,B1,widththreshold);
    /* now compute A2 * B2 and add it to C */
    matrix_multiplication_dc(level+1,C,A2,B2,widththreshold);
    matrix_delete(A1);
    matrix_delete(A2);
    matrix_delete(B1);
    matrix_delete(B2);
    SHOWCASEEND;
  }
}

typedef struct
{
  Matrix *A, *B, *C;
} Submatrixtask;

#define CHECKSPACE(SPACE,TYPE,NEXTFREE,ALLOCATED)\
        if ((NEXTFREE) >= (ALLOCATED))\
        {\
          ALLOCATED = (ALLOCATED) * 1.2 + 256;\
          SPACE = (TYPE *) realloc(SPACE,(ALLOCATED) * sizeof (TYPE));\
          assert((SPACE) != NULL);\
        }

#define COND_MPOINTER_LIST_APPEND(IS_TMP,M)\
        if (IS_TMP)\
        {\
          CHECKSPACE(mpointer_list,Matrix *,mpointer_list_nextfree,\
                     mpointer_list_allocated);\
          mpointer_list[mpointer_list_nextfree++] = (Matrix *) (M);\
        }

#define APPEND_SUBMATRIX_TASK(LIST,NEXTFREE,ALLOCATED,MC,MA,MB)\
        CHECKSPACE(LIST,Submatrixtask,NEXTFREE,ALLOCATED);\
        LIST[NEXTFREE].C = (Matrix *) (MC);\
        LIST[NEXTFREE].A = (Matrix *) (MA);\
        LIST[NEXTFREE].B = (Matrix *) (MB);\
        (NEXTFREE)++

#define PUSH_SUBMATRIX_TASK(MC,C_IS_TMP,MA,A_IS_TMP,MB,B_IS_TMP)\
        APPEND_SUBMATRIX_TASK(stack,stack_nextfree,stack_allocated,MC,MA,MB);\
        COND_MPOINTER_LIST_APPEND(C_IS_TMP,MC);\
        COND_MPOINTER_LIST_APPEND(A_IS_TMP,MA);\
        COND_MPOINTER_LIST_APPEND(B_IS_TMP,MB)

void matrix_multiplication_dc_iter(Matrix *C,
                                   const Matrix *A,const Matrix *B,
                                   size_t widththreshold)
{
  Submatrixtask *stack = NULL,
                *mult_task_list = NULL;
  Matrix **mpointer_list = NULL;
  size_t stack_nextfree = 0, stack_allocated = 0,
         mult_task_list_nextfree = 0, mult_task_list_allocated = 0,
         mpointer_list_nextfree = 0, mpointer_list_allocated = 0;

  PUSH_SUBMATRIX_TASK(C,false,A,false,B,false);
  while (stack_nextfree > 0)
  {
    Submatrixtask task = stack[--stack_nextfree];

    EXPECTED_EQUALITY(task.A->columns,task.B->rows);
    EXPECTED_EQUALITY(task.C->rows,task.A->rows);
    EXPECTED_EQUALITY(task.C->columns,task.B->columns);
    size_t m = task.A->rows;
    size_t n = task.A->columns;
    size_t l = task.B->columns;
    size_t maxwidth = MAX2(m,MAX2(n,l));
    if (maxwidth <= widththreshold)
    {
      APPEND_SUBMATRIX_TASK(mult_task_list,mult_task_list_nextfree,
                            mult_task_list_allocated,task.C,task.A,task.B);
      /*
      printf("matrix C: %lu X %lu\n",task.C->rows,task.C->columns);
      printf("matrix A: %lu X %lu\n",task.A->rows,task.A->columns);
      printf("matrix B: %lu X %lu\n",task.B->rows,task.B->columns);
      */
    } else
    {
      if (maxwidth == m)
      {
        Matrix *A1, *A2, *C1, *C2;

        A1 = matrix_h_split_upper(task.A);
        A2 = matrix_h_split_lower(task.A);
        C1 = matrix_h_split_upper(task.C);
        C2 = matrix_h_split_lower(task.C);
        PUSH_SUBMATRIX_TASK(C1,true,A1,true,task.B,false);
        PUSH_SUBMATRIX_TASK(C2,true,A2,true,task.B,false);
      } else
      {
        if (maxwidth == l)
        {
          Matrix *B1, *B2, *C1, *C2;

          B1 = matrix_v_split_left(task.B);
          B2 = matrix_v_split_right(task.B);
          C1 = matrix_v_split_left(task.C);
          C2 = matrix_v_split_right(task.C);
          PUSH_SUBMATRIX_TASK(C1,true,task.A,false,B1,true);
          PUSH_SUBMATRIX_TASK(C2,true,task.A,false,B2,true);
        } else
        {
          Matrix *A1, *A2, *B1, *B2;

          assert (maxwidth == n);
          A1 = matrix_v_split_left(task.A);
          A2 = matrix_v_split_right(task.A);
          B1 = matrix_h_split_upper(task.B);
          B2 = matrix_h_split_lower(task.B);
          PUSH_SUBMATRIX_TASK(task.C,false,A1,true,B1,true);
          /* now add A2 + B2 to C */
          PUSH_SUBMATRIX_TASK(task.C,false,A2,true,B2,true);
        }
      }
    }
  }
  printf("# number of multiplication tasks\t%lu\n",mult_task_list_nextfree);
  for (size_t idx = 0; idx < mult_task_list_nextfree; idx++)
  {
    Submatrixtask *mptr = &mult_task_list[idx];
    matrix_multiplication(mptr->C,mptr->A,mptr->B);
  }
  printf("# number of temporary matrix pointers\t%lu\n",mpointer_list_nextfree);
  for (size_t idx = 0; idx < mpointer_list_nextfree; idx++)
  {
    matrix_delete(mpointer_list[idx]);
  }
  free(mult_task_list);
  free(mpointer_list);
  free(stack);
}

typedef struct
{
  const Matrix *A, *B;
  Matrix *C;
  size_t widththreshold;
  pthread_t ident;
} MMThreadinfo;

static void *matrix_multiplication_dc_runner(void *tinfo)
{
  MMThreadinfo *threadinfo = (MMThreadinfo *) tinfo;

  matrix_multiplication_dc(1,threadinfo->C,threadinfo->A,
                           threadinfo->B,threadinfo->widththreshold);
  return NULL;
}

static void dc_thread_create(MMThreadinfo *threadinfo,
                             const Matrix *A,
                             const Matrix *B,
                             Matrix *C,
                             size_t widththreshold)
{
  threadinfo->A = A;
  threadinfo->B = B;
  threadinfo->C = C;
  threadinfo->widththreshold = widththreshold;
  pthread_create(&threadinfo->ident,NULL,
                 matrix_multiplication_dc_runner,threadinfo);
}

void matrix_multiplication_dc_2threads(
            __attribute__ ((unused)) int level,
            Matrix *C,const Matrix *A,const Matrix *B,
            size_t widththreshold)
{
  MMThreadinfo threadinfo_tab[2];

  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);
  size_t m = A->rows;
  size_t n = A->columns;
  size_t l = B->columns;
  size_t maxwidth = MAX2(m,MAX2(n,l));
  if (maxwidth <= widththreshold)
  {
    SHOWCASE("1");
    matrix_multiplication(C,A,B);
    directmultiplications++;
    SHOWCASEEND;
    return;
  }
  if (maxwidth == m)
  {
    Matrix *A1, *A2, /* source submatrix */
           *C1, *C2; /* target submatrix */

    SHOWCASE("2a");
    A1 = matrix_h_split_upper(A);
    A2 = matrix_h_split_lower(A);
    C1 = matrix_h_split_upper(C);
    C2 = matrix_h_split_lower(C);
    dc_thread_create(&threadinfo_tab[0],A1,B,C1,
                     widththreshold);
    dc_thread_create(&threadinfo_tab[1],A2,B,C2,
                     widththreshold);
    pthread_join(threadinfo_tab[0].ident,NULL);
    pthread_join(threadinfo_tab[1].ident,NULL);
    matrix_delete(A1);
    matrix_delete(A2);
    matrix_delete(C1);
    matrix_delete(C2);
    SHOWCASEEND;
    return;
  }
  if (maxwidth == l)
  {
    Matrix *B1, *B2, /* source submatrix */
           *C1, *C2; /* target submatrix */

    SHOWCASE("2b");
    B1 = matrix_v_split_left(B);
    B2 = matrix_v_split_right(B);
    C1 = matrix_v_split_left(C);
    C2 = matrix_v_split_right(C);
    dc_thread_create(&threadinfo_tab[0],A,B1,C1,
                     widththreshold);
    dc_thread_create(&threadinfo_tab[1],A,B2,C2,
                     widththreshold);
    pthread_join(threadinfo_tab[0].ident,NULL);
    pthread_join(threadinfo_tab[1].ident,NULL);
    matrix_delete(B1);
    matrix_delete(B2);
    matrix_delete(C1);
    matrix_delete(C2);
    SHOWCASEEND;
    return;
  }
  { /* maxwidth == n */
    Matrix *A1, *A2, *B1, *B2, *Ctmp;

    SHOWCASE("2c");
    A1 = matrix_v_split_left(A);
    A2 = matrix_v_split_right(A);
    B1 = matrix_h_split_upper(B);
    B2 = matrix_h_split_lower(B);
    Ctmp = matrix_new(C->rows,C->columns);
    dc_thread_create(&threadinfo_tab[0],A1,B1,C,widththreshold);
    dc_thread_create(&threadinfo_tab[1],A2,B2,Ctmp,widththreshold);
    pthread_join(threadinfo_tab[0].ident,NULL);
    pthread_join(threadinfo_tab[1].ident,NULL);
    matrix_addition_inplace(C,Ctmp);
    matrix_delete(A1);
    matrix_delete(A2);
    matrix_delete(B1);
    matrix_delete(B2);
    matrix_delete(Ctmp);
    SHOWCASEEND;
  }
}

size_t directmultiplications_dc(void)
{
#ifdef DEBUG
  return 0;
#else
  return directmultiplications;
#endif
}

size_t recursive_calls_dc(void)
{
  return recursive_calls;
}
