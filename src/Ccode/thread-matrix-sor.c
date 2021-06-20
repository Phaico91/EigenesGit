#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "array2dim.h"

static double combine_values(double a,double b,double c,double d)
{
  return atan(a * a + b * b + c * c + d * d);
}

static void matrix_eval(double **dest,double **src,
                        unsigned long rowstart,
                        unsigned long rowend,
                        unsigned long columns)
{
  unsigned long r, c;
  for (r = rowstart; r < rowend; r++)
    for (c = 1; c < columns; c++)
      dest[r][c] = combine_values(src[r][c-1],src[r][c+1],
                                  src[r-1][c],src[r+1][c]);
}

static void matrix_copy(double **dest,
                        double **src,
                        unsigned long rowstart,
                        unsigned long rowend,
                        unsigned long columns)
{
  unsigned long row, col;

  for (row = rowstart; row < rowend; row++)
  {
    for (col = 1; col < columns; col++)
    {
      dest[row][col] = src[row][col];
    }
  }
}

static void matrix_eval_seq_iter(unsigned long iterations,
                                 double **matrix,
                                 unsigned long rows,
                                 unsigned long columns)
{
  unsigned long iter;
  double **temp;

  array2dim_malloc(temp, double, rows+2, columns+2);
  for (iter = 0; iter < iterations; iter++)
  {
    matrix_eval(temp,matrix,1,rows,columns);
    matrix_copy(matrix,temp,1,rows,columns);
  }
  array2dim_delete(temp);
}

typedef struct
{
  unsigned long columns; /* number of columns */
  double **matrix,   /* input and output matrix */
         **temp;     /* temporary matrix of same size */
} Matrixresources;

typedef struct
{
  unsigned long rowstart, rowend; /* range of rows */
                                  /* processed by thread */
  pthread_t thread_id;            /* with this identifier */
  Matrixresources *mrptr;         /* ref. to shared data */
} Partinfo;

static void *matrix_eval_threadfun(void *vpartinfo)
{
  Partinfo *partinfo = (Partinfo *) vpartinfo;
  matrix_eval(partinfo->mrptr->temp, /* dest */
              partinfo->mrptr->matrix, /* src */
              partinfo->rowstart,partinfo->rowend,
              partinfo->mrptr->columns);
  return NULL;
}

static void *matrix_copy_threadfun(void *vpartinfo)
{
  Partinfo *partinfo = (Partinfo *) vpartinfo;
  matrix_copy(partinfo->mrptr->matrix, /* dest */
              partinfo->mrptr->temp,   /* src */
              partinfo->rowstart,partinfo->rowend,
              partinfo->mrptr->columns);
  return NULL;
}

static void matrix_eval_step(Partinfo *partinfo_tab,
                             unsigned long numthreads)
{
  unsigned long t;

  for (t = 0; t < numthreads; t++)
    pthread_create(&partinfo_tab[t].thread_id,NULL,
                   matrix_eval_threadfun,partinfo_tab + t);
  for (t = 0; t < numthreads; t++)
    pthread_join(partinfo_tab[t].thread_id,NULL);
  /* Now all rows of the matrix are computed and program runs
     with a single thread, before it is threaded again. */
  for (t = 0; t < numthreads; t++)
    pthread_create(&partinfo_tab[t].thread_id,NULL,
                   matrix_copy_threadfun,partinfo_tab + t);
  for (t = 0; t < numthreads; t++)
    pthread_join(partinfo_tab[t].thread_id,NULL);
}

#define MIN(X,Y)  ((X) < (Y) ? (X) : (Y))
static Partinfo *partinfo_new(unsigned long numparts,
                              Matrixresources *mrptr,
                              unsigned long rows)
{
  unsigned long p, width = rows/numparts;
  Partinfo *partinfo_tab;

  partinfo_tab = malloc(numparts * sizeof *partinfo_tab);
  assert(numparts > 1 && partinfo_tab != NULL);
  for (p = 0; p < numparts; p++)
  {
    partinfo_tab[p].mrptr = mrptr; /* shared resource */
    partinfo_tab[p].rowstart = 1 + p * width;
    partinfo_tab[p].rowend = MIN(1 + (p+1) * width,rows+1);
  }
  return partinfo_tab;
}

static void matrix_eval_threaded_iter(unsigned long numthreads,
                                      unsigned long iterations,
                                      double **matrix,
                                      unsigned long rows,
                                      unsigned long columns)
{
  Matrixresources mr;
  Partinfo *partinfo_tab;
  unsigned long iter;

  mr.matrix = matrix;
  mr.columns = columns;
  array2dim_malloc(mr.temp,double,rows+2, columns+2);
  partinfo_tab = partinfo_new(numthreads, &mr, rows);
  for (iter = 0; iter < iterations; iter++)
    matrix_eval_step(partinfo_tab,numthreads);
  free(partinfo_tab);
  array2dim_delete(mr.temp);
}

static void usage(void)
{
  fprintf(stderr,"Usage: thread-matrix-sor.x <threads> <iterations> "
                 "<rows> <columns>\n");
  exit(EXIT_FAILURE);
}

static unsigned long parse_arg(const char *arg)
{
  long readlong;

  if (sscanf(arg,"%ld",&readlong) != 1 || readlong <= 0)
  {
    usage();
  }
  return (unsigned long) readlong;
}

static double **matrix_new_random(unsigned long rows, unsigned long columns)
{
  double **matrix;
  unsigned long row, col;

  array2dim_malloc(matrix,double,rows+2, columns+2);
  for (row = 0; row <= rows+1; row++)
  {
    matrix[row][0] = 0.0;
  }
  for (col = 0; col <= columns+1; col++)
  {
    matrix[0][col] = 0.0;
  }
  for (row = 1; row <= rows; row++)
  {
    for (col = 1; col <= columns; col++)
    {
      matrix[row][col] = drand48();
    }
  }
  return matrix;
}

static void matrix_show(double **matrix,unsigned long rows,
                        unsigned long columns)
{
  unsigned long row, col;

  for (row = 0; row < rows; row++)
  {
    for (col = 0; col < columns; col++)
    {
      printf("%.6f%c",matrix[row][col],col < columns - 1 ? ' ' : '\n');
    }
  }
}

int main(int argc, char *argv[])
{
  unsigned long numthreads, iterations, rows, columns;
  double **matrix;

  if (argc != 5) usage();
  numthreads = parse_arg(argv[1]); /* numthreads <- argv[1] */
  iterations = parse_arg(argv[2]); /* iterations <- argv[2] */
  rows = parse_arg(argv[3]);       /* rows <- argv[3] */
  columns = parse_arg(argv[4]);    /* columns <- argv[4] */
  srand48(366292341);          /* init random num generator */
  matrix = matrix_new_random(rows,columns);
  if (numthreads == 1 || numthreads >= rows)
    matrix_eval_seq_iter(iterations, matrix, rows, columns);
  else
    matrix_eval_threaded_iter(numthreads,iterations, matrix,
                              rows, columns);
  matrix_show(matrix,rows, columns);
  array2dim_delete(matrix);
  return EXIT_SUCCESS;
}
