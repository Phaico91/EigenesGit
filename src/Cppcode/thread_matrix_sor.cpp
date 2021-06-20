#include <cmath>
#include <cassert>
#include <cassert>
#include <thread>
#include <algorithm>
#include <vector>
#include <utility>  /* for pair */
#include "matrixclass.hpp"

static double combine_values(double a,double b,
                             double c,double d) {
  return atan(a * a + b * b + c * c + d * d);
}
using Matrix_d = Matrix<double>;

static void matrix_eval(Matrix_d &dest,
                        const Matrix_d &src,
                        size_t rowstart, size_t rowend,
                        size_t columns)
{
  for (size_t r = rowstart; r < rowend; r++)
    for (size_t c = 1; c < columns; c++)
      dest.set(r,c,combine_values(src(r,c-1),src(r,c+1),
                                  src(r-1,c),src(r+1,c)));
}

static void matrix_copy(Matrix_d &dest,
                        const Matrix_d &src,
                        size_t rowstart,
                        size_t rowend,
                        size_t columns)
{
  for (size_t row = rowstart; row < rowend; row++)
  {
    for (size_t col = 1; col < columns; col++)
    {
      dest.set(row,col,src(row,col));
    }
  }
}

static void matrix_eval_seq_iter(size_t iterations,
                                 Matrix_d &matrix,
                                 size_t rows,
                                 size_t columns)
{
  const size_t rowstart = 1, rowend = rows;
  Matrix_d temp(rows+2,columns+2);

  for (size_t iter = 0; iter < iterations; iter++)
  {
    matrix_eval(temp,matrix,rowstart,rowend,columns);
    matrix_copy(matrix,temp,rowstart,rowend,columns);
  }
}

using MatrixFunction
  = void (*)(Matrix_d &dest,      /* values will be written */
             const Matrix_d &src, /* will only be read */
             size_t rowstart,     /* first row to evaluate */
             size_t rowend,       /* after last row to eval */
             size_t columns);     /* number of rows */

class PartInfoTab {
  private:
    size_t numparts, *rowstarts, *rowends;
    std::thread *threads;
  public:
    PartInfoTab(size_t _numparts,size_t rows) { /* constructor */
      const size_t height = rows/_numparts;
      numparts = _numparts;
      threads = new std::thread [_numparts];
      rowstarts = new size_t [_numparts];
      rowends = new size_t [_numparts];
      for (size_t p = 0; p < _numparts; p++) {
        rowstarts[p] = 1 + p * height;
        rowends[p] = std::min(1 + (p+1) * height,rows+1);
      } /* std::min only affects last part */
    }
    ~PartInfoTab(void) {
      delete[] threads; delete[] rowstarts; delete[] rowends;
    }
  private:
    void run_threads(MatrixFunction f,Matrix_d &dest,
                     const Matrix_d &src,size_t columns)
    {
      for (size_t p = 0; p < numparts; p++)
        threads[p] = std::thread(f,std::ref(dest),
                                 std::ref(src),rowstarts[p],
                                 rowends[p],columns);
      for (size_t p = 0; p < numparts; p++) threads[p].join();
    }
  public:
    void matrix_eval_step(Matrix_d &matrix,
                          Matrix_d &temp,
                          size_t columns)
    {
      run_threads(matrix_eval,temp,matrix,columns);
      /* Now all rows of the matrix are computed and program runs
        with a single thread, before it is threaded again. */
      run_threads(matrix_copy,matrix,temp,columns);
    }
}; /* end of class PartInfoTab */

static void matrix_eval_threaded_iter(size_t numthreads,
                                      size_t iterations,
                                      Matrix_d &matrix,
                                      size_t rows,
                                      size_t columns)
{
  PartInfoTab partinfo_tab(numthreads,rows);
  Matrix_d temp(rows+2,columns+2);

  for (size_t iter = 0; iter < iterations; iter++)
    partinfo_tab.matrix_eval_step(matrix,temp,columns);
}

static void usage(const char *progname)
{
  fprintf(stderr,"Usage: %s <threads> <iterations> "
                 "<rows> <columns>\n",progname);
  exit(EXIT_FAILURE);
}

static size_t parse_arg(const char *progname,const char *arg)
{
  long readlong;

  if (sscanf(arg,"%ld",&readlong) != 1 || readlong <= 0)
  {
    usage(progname);
  }
  return (size_t) readlong;
}

int main(int argc, char *argv[])
{
  size_t numthreads, iterations, rows, columns;

  if (argc != 5) usage(argv[0]);  /* usage & parse_arg  not shown */
  numthreads = parse_arg(argv[0],argv[1]); /* numthreads */
  iterations = parse_arg(argv[0],argv[2]); /* iterations */
  rows = parse_arg(argv[0],argv[3]);       /* rows */
  columns = parse_arg(argv[0],argv[4]);    /* columns */
  Matrix_d matrix(rows+2,columns+2); /* add 2 extra rows and cols */
  matrix.init_random(0,1.0); /* random values */
  if (numthreads == 1 || numthreads >= rows)
    matrix_eval_seq_iter(iterations, matrix, rows, columns);
  else
    matrix_eval_threaded_iter(numthreads,iterations, matrix,
                              rows, columns);
  matrix.show();
  return EXIT_SUCCESS;
}
