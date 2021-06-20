#include <cstdlib>
#include <cstdbool>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

template<typename T>
static T **matrix_rowptr(size_t rows,
                         size_t origcols,
                         T *firstsubrow) {
  T **M = new T* [rows];
  M[0] = firstsubrow; /* pointer to the first row of submatrix */
  for (size_t idx = 1; idx < rows; idx++)
    M[idx] = M[idx-1] + origcols; /* consecutive rows at */
  return M;                       /* distance origcols */
}

template <typename T>
class Matrix {
   private:
     bool hasownvalues;
     size_t n_rows,      /* number of rows */
            n_cols,      /* number of columns */
            rowstart,    /* first row of submatrix */
            columnstart, /* first column of submatrix */
            origcols;    /* refers to original matrix */
     T * *M;  // array of row pointers
   public:
     size_t rows(void) const { return n_rows; }
     size_t columns(void) const { return n_cols; }
     Matrix(size_t r, size_t c) { // constructor
       assert(r > 0 && c > 0);
       hasownvalues = true;
       n_rows = r;
       n_cols = origcols = c;
       rowstart = columnstart = 0;
       M = new T* [n_rows]; // array of row pointers
       M[0] = new T [n_rows * n_cols]; // array of values
       for (size_t i = 1; i < n_rows; i++)
         M[i] = M[i-1] + n_cols; // row ptr at distance n_cols
     }
     Matrix(Matrix *parent,bool horiz,bool first)
     {
       hasownvalues = false;
       if (horiz)
       {
         assert(parent->rows() > 1);
         origcols = parent->origcols;
         n_cols = parent->columns();
         columnstart = parent->columnstart;
         if (first)
         {
           n_rows = parent->rows()/2 + (parent->rows() % 2); /* ceil */
           rowstart = parent->rowstart;
           M = matrix_rowptr(n_rows,
                             origcols,
           /* firstsubrow */ parent->M[0]);
         } else
         {
           n_rows = parent->rows()/2;
           size_t firstrows = parent->rows() - n_rows;
           rowstart = parent->rowstart + firstrows;
           M = matrix_rowptr(n_rows,
                             origcols,
            /*firstsubrow:*/ parent->M[firstrows]);
         }
       } else
       {
         assert(parent->columns() > 1);
         origcols = parent->origcols;
         n_rows = parent->rows();
         rowstart = parent->rowstart;
         if (first)
         {
           n_cols = parent->columns()/2 +
                    (parent->columns() % 2); /* ceil */
           columnstart = parent->columnstart;
           M = matrix_rowptr(n_rows,
                             origcols,
            /*firstsubrow:*/ parent->M[0]);
         } else
         {
           n_cols = parent->columns()/2;
           size_t fstcols = parent->columns() - n_cols;
           columnstart = parent->columnstart + fstcols;
           M = matrix_rowptr(n_rows,
                             origcols,
          /*firstsubrow:*/   parent->M[0] + fstcols);
         }
       }
     }
     ~Matrix() {  // destructor
       if (hasownvalues)
       {
         delete [] M[0]; // delete space for elements
       }
       delete [] M;    // delete space for row ptr
     }
     T operator()(size_t i, size_t j) const { // M(i,j)
       return M[i][j];
     }
     void set(size_t i, size_t j,T v) { M[i][j] = v; }
     void add(size_t i, size_t j,T v) { M[i][j] += v; }
     void show() const { // pretty print matrix
       for (size_t i = 0; i < n_rows; i++) {
         for (size_t j = 0; j < n_cols; j++) {
           std::cout << std::fixed << std::setprecision(2)
                     << M[i][j] << "\t";
         }
         std::cout << std::endl;
       }
     }
     void fill(T value)
     {
       for (size_t i = 0; i < n_rows; i++)
         for (size_t j = 0; j < n_cols; j++)  M[i][j] = value;
     }
     void init_random(T min,T max) {
       assert(min <= max);
       const double rng = (double) (max - min);
       std::random_device r;
       std::default_random_engine engine(r());
       std::uniform_real_distribution<double> dist(0.0,1.0);
       for (size_t i = 0; i < n_rows; i++)
         for (size_t j = 0; j < n_cols; j++) {
           M[i][j] = (T) ((double) min + dist(engine) * rng);
         }
     }
     Matrix<T> matrix_transposition(const Matrix &A)
     {
       Matrix B(A.columns(),A.rows());
       for (size_t r = 0; r < A->rows(); r++)
       {
         for (size_t c = 0; c < A->columns(); c++)
         {
           B(c,r) = A(r,c);
         }
       }
       return B;
     }
};

template <typename T>
bool operator==(const Matrix<T> &A,const Matrix<T> &B)
{
  assert(A.rows() == B.rows());
  assert(A.columns() == B.columns());
  for (size_t i = 0; i < A.rows(); i++)
  {
    for (size_t j = 0; j < A.columns(); j++)
    {
      if (A(i,j) != B(i,j))
      {
        std::cout << std::fixed << std::setprecision(2) << A(i,j)
                  << " != "
                  << std::fixed << std::setprecision(2) << B(i,j)
                  << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

template <typename T>
Matrix<T> operator*(const Matrix<T> &A,const Matrix<T> &B)
{
  assert(A.columns() == B.rows());
  Matrix<T> C(A.rows(),B.columns());
  for (size_t i = 0; i < C.rows(); i++)
  {
    for (size_t j = 0; j < C.columns(); j++)
    {
      T sum = (T) 0;

      for (size_t k = 0; k < A.columns(); k++)
      {
        sum += A(i,k) * B(k,j);
      }
      C.set(i,j,sum);
    }
  }
  return C;
}

template <typename T>
Matrix<T> &operator+(Matrix<T> &A,const Matrix<T> &B)
{
  assert(A.rows() == B.rows());
  assert(A.columns() == B.columns());
  Matrix<T> C(A.rows(),A.columns());
  for (size_t i = 0; i < C.rows(); i++)
  {
    for (size_t j = 0; j < C.columns(); j++)
    {
      C.set(i,j) = A(i,j) + B(i,j);
    }
  }
  return C;
}

template <typename T>
void matrix_multiplication(Matrix<T> *C,Matrix<T> *A,Matrix<T> *B)
{
  assert(A->columns() == B->rows());
  assert(C->rows() == A->rows());
  assert(C->columns() == B->columns());
  for (size_t i = 0; i < C->rows(); i++)
  {
    for (size_t j = 0; j < C->columns(); j++)
    {
      T sum = (T) 0;

      for (size_t k = 0; k < A->columns(); k++)
      {
        sum += (*A)(i,k) * (*B)(k,j);
      }
      C->add(i,j,sum);  /* we always add value */
    }
  }
}

template<typename T>
class Submatrixtask
{
  public:
  Matrix<T> *A, *B, *C;
  Submatrixtask(Matrix<T> *_C,Matrix<T> *_A,Matrix<T> *_B)
  {
    C = _C;
    A = _A;
    B = _B;
  }
};

#define COND_MPOINTER_LIST_APPEND(IS_TMP,M)\
        if constexpr (IS_TMP)\
        {\
          mpointer_list.push_back(M);\
        }

#define PUSH_SUBMATRIX_TASK(MC,C_IS_TMP,MA,A_IS_TMP,MB,B_IS_TMP)\
        stack.push_back(Submatrixtask(MC,MA,MB));\
        COND_MPOINTER_LIST_APPEND(C_IS_TMP,MC);\
        COND_MPOINTER_LIST_APPEND(A_IS_TMP,MA);\
        COND_MPOINTER_LIST_APPEND(B_IS_TMP,MB)

template<typename T>
void matrix_multiplication_dc_iter(size_t widththreshold,Matrix<T> *C,
                                   Matrix<T> *A,Matrix<T> *B)
{
  std::vector<Submatrixtask<T>> stack{},
                                mult_task_list{};
  std::vector<Matrix<T> *> mpointer_list{};

  PUSH_SUBMATRIX_TASK(C,false,A,false,B,false);
  while (!stack.empty())
  {
    Submatrixtask<T> task = stack.back();
    stack.pop_back();

    assert(task.A->columns() == task.B->rows());
    assert(task.C->rows() == task.A->rows());
    assert(task.C->columns() == task.B->columns());
    size_t m = task.A->rows();
    size_t n = task.A->columns();
    size_t l = task.B->columns();
    size_t maxwidth = std::max(m,std::max(n,l));
    if (maxwidth <= widththreshold)
    {
      mult_task_list.push_back(Submatrixtask<T>(task.C,task.A,task.B));
    } else
    {
      if (maxwidth == m)
      {
        Matrix<T> *A1, *A2, *C1, *C2;

        A1 = new Matrix<T>(task.A,true,true);  /* h_split_upper */
        A2 = new Matrix<T>(task.A,true,false); /* h_split_lower */
        C1 = new Matrix<T>(task.C,true,true);  /* h_split_upper */
        C2 = new Matrix<T>(task.C,true,false); /* h_split_lower */
        PUSH_SUBMATRIX_TASK(C1,true,A1,true,task.B,false);
        PUSH_SUBMATRIX_TASK(C2,true,A2,true,task.B,false);
      } else
      {
        if (maxwidth == l)
        {
          Matrix<T> *B1, *B2, *C1, *C2;

          B1 = new Matrix<T>(task.B,false,true);  /* v_split_left */
          B2 = new Matrix<T>(task.B,false,false); /* v_split_right */
          C1 = new Matrix<T>(task.C,false,true);  /* v_split_left */
          C2 = new Matrix<T>(task.C,false,false); /* v_split_right */
          PUSH_SUBMATRIX_TASK(C1,true,task.A,false,B1,true);
          PUSH_SUBMATRIX_TASK(C2,true,task.A,false,B2,true);
        } else
        {
          Matrix<T> *A1, *A2, *B1, *B2;

          assert (maxwidth == n);
          A1 = new Matrix<T>(task.A,false,true);  /* v_split_left */
          A2 = new Matrix<T>(task.A,false,false); /* v_split_right */
          B1 = new Matrix<T>(task.B,true,true);   /* h_split_upper */
          B2 = new Matrix<T>(task.B,true,false);  /* h_split_lower */
          PUSH_SUBMATRIX_TASK(task.C,false,A1,true,B1,true);
          /* now add A2 + B2 to C */
          PUSH_SUBMATRIX_TASK(task.C,false,A2,true,B2,true);
        }
      }
    }
  }
  std::cout << "# number of multiplication tasks\t" << mult_task_list.size()
            << std::endl;
  for (auto &&mt : mult_task_list)
  {
    matrix_multiplication(mt.C,mt.A,mt.B);
  }
  std::cout << "# number of temporary matrix pointers\t" << mpointer_list.size()
            << std::endl;
  for (auto &&mptr : mpointer_list)
  {
    delete mptr;
  }
}

template<typename T>
void matrix_example_dc(size_t m,size_t n,size_t k,
                       T min,T max)
{
  Matrix<T> A = Matrix<T>(m,n);
  Matrix<T> B = Matrix<T>(n,k);
  A.init_random(min,max);
  B.init_random(min,max);
  Matrix<T> C(m,k);
  C.fill(0);
  matrix_multiplication_dc_iter<T>(10,&C,&A,&B);
  C.show();
}

template<typename T>
void matrix_example(size_t m,size_t n,size_t k,
                    T min,T max)
{
  Matrix<T> A = Matrix<T>(m,n);
  Matrix<T> B = Matrix<T>(n,k);
  A.init_random(min,max);
  B.init_random(min,max);
  Matrix<T> C = A * B;
  C.show();
}

int main(void)
{
  matrix_example<double>(4,4,4,0.0,1.0);
  matrix_example<int>(4,4,4,0,10);
  matrix_example_dc<double>(4,4,4,0.0,1.0);
  matrix_example_dc<int>(4,4,4,0,10);
  matrix_example_dc<int>(100,100,100,0,10);
  return EXIT_SUCCESS;
}
