#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <thread>
#include "pfn_thread_pool.hpp"
#include "matrixclass.hpp"
#include "matrixmult_opt.h"

template <class T>
void matrix_multiplication(Matrix<T> *C, const Matrix<T> *A,
                           const Matrix<T> *B)
{
  assert(A->columns() == B->rows() && C->rows() == A->rows());
  assert(C->columns() == B->columns());
  for (size_t i = 0; i < C->rows(); i++)
  {
    for (size_t j = 0; j < C->columns(); j++)
    {
      T sum = C->get(i, j);

      for (size_t k = 0; k < A->columns(); k++)
      {
        sum += A->get(i, k) * B->get(k, j);
      }
      C->set(i, j, sum);
    }
  }
}

template <class T>
void matrix_multiplication_transposed(Matrix<T> *C, const Matrix<T> &A,
                                      const Matrix<T> &B)
{
  assert(A.columns() == B.rows());
  assert(C->rows() == A.rows());
  assert(C->columns() == B.columns());
  Matrix<T> *BT = B.transpose();
  for (size_t i = 0; i < C->rows(); i++)
  {
    for (size_t j = 0; j < C->columns(); j++)
    {
      T sum = C->get(i, j);

      for (size_t k = 0; k < A.columns(); k++)
      {
        sum += A(i, k) * BT->get(j, k);
      }
      C->set(i, j, sum);
    }
  }
  delete BT;
}

template <class T>
void matrix_multiplication_dc(Matrix<T> *C,
                              const Matrix<T> *A,
                              const Matrix<T> *B,
                              size_t width_threshold)
{
  size_t m = A->rows();
  size_t n = A->columns();
  size_t l = B->columns();
  assert(n == B->rows() && m == C->rows() && l == C->columns());
  size_t maxwidth = std::max(m, std::max(n, l));
  if (maxwidth <= width_threshold)
  {
    matrix_multiplication(C, A, B);
    return;
  }
#define H_SPLIT_UPPER true,true
#define H_SPLIT_LOWER true,false
#define V_SPLIT_LEFT  false,true
#define V_SPLIT_RIGHT false,false
  if (maxwidth == m) /* case 2a */
  {
    Matrix<T> A1 = Matrix<T>(A,H_SPLIT_UPPER);
    Matrix<T> A2 = Matrix<T>(A,H_SPLIT_LOWER);
    Matrix<T> C1 = Matrix<T>(C,H_SPLIT_UPPER);
    Matrix<T> C2 = Matrix<T>(C,H_SPLIT_LOWER);
    matrix_multiplication_dc(&C1, &A1, B, width_threshold);
    matrix_multiplication_dc(&C2, &A2, B, width_threshold);
    return;
  }
  if (maxwidth == l) /* case 2b */
  {
    Matrix<T> B1 = Matrix<T>(B,V_SPLIT_LEFT);
    Matrix<T> B2 = Matrix<T>(B,V_SPLIT_RIGHT);
    Matrix<T> C1 = Matrix<T>(C,V_SPLIT_LEFT);
    Matrix<T> C2 = Matrix<T>(C,V_SPLIT_RIGHT);
    matrix_multiplication_dc(&C1, A, &B1, width_threshold);
    matrix_multiplication_dc(&C2, A, &B2, width_threshold);
    return;
  }
  assert(maxwidth == n);
  Matrix<T> A1 = Matrix<T>(A,V_SPLIT_LEFT); /* case 2c */
  Matrix<T> A2 = Matrix<T>(A,V_SPLIT_RIGHT);
  Matrix<T> B1 = Matrix<T>(B,H_SPLIT_UPPER);
  Matrix<T> B2 = Matrix<T>(B,H_SPLIT_LOWER);
  matrix_multiplication_dc(C, &A1, &B1, width_threshold);
  /* now compute A2 * B2 and add it to C */
  matrix_multiplication_dc(C, &A2, &B2, width_threshold);
} /* end of matrix_multiplication_dc */

#ifdef NEWDCVERSION
template<typename T>
using MultiplicationTask = std::tuple<Matrix<T> *,const Matrix<T> *,
                                      const Matrix<T> *>;

template <class T>
static void matrix_multiplication_dc2(
                               std::vector<MultiplicationTask<T>> &mtasks,
                               std::vector<Matrix<T> *> &mptr_list,
                               size_t wth,
                               Matrix<T> *C,
                               const Matrix<T> *A,
                               const Matrix<T> *B)
{
  size_t m = A->rows();
  size_t n = A->columns();
  size_t l = B->columns();
  assert(n == B->rows() && m == C->rows() && l == C->columns());
  size_t maxwidth = std::max(m, std::max(n, l));
  if (maxwidth <= wth)
  {
    mtasks.push_back({C,A,B});
    return;
  }
#define H_SPLIT_UPPER true,true
#define H_SPLIT_LOWER true,false
#define V_SPLIT_LEFT  false,true
#define V_SPLIT_RIGHT false,false
  if (maxwidth == m) /* case 2a */
  {
    Matrix<T> *A1 = new Matrix<T>(*A,H_SPLIT_UPPER);
    Matrix<T> *A2 = new Matrix<T>(*A,H_SPLIT_LOWER);
    Matrix<T> *C1 = new Matrix<T>(*C,H_SPLIT_UPPER);
    Matrix<T> *C2 = new Matrix<T>(*C,H_SPLIT_LOWER);
    matrix_multiplication_dc2(mtasks,mptr_list,wth,C1, A1, B);
    matrix_multiplication_dc2(mtasks,mptr_list,wth,C2, A2, B);
    mptr_list.push_back(A1);
    mptr_list.push_back(A2);
    mptr_list.push_back(C1);
    mptr_list.push_back(C2);
    return;
  }
  if (maxwidth == l) /* case 2b */
  {
    Matrix<T> *B1 = new Matrix<T>(*B,V_SPLIT_LEFT);
    Matrix<T> *B2 = new Matrix<T>(*B,V_SPLIT_RIGHT);
    Matrix<T> *C1 = new Matrix<T>(*C,V_SPLIT_LEFT);
    Matrix<T> *C2 = new Matrix<T>(*C,V_SPLIT_RIGHT);
    matrix_multiplication_dc2(mtasks,mptr_list,wth,C1, A, B1);
    matrix_multiplication_dc2(mtasks,mptr_list,wth,C2, A, B2);
    mptr_list.push_back(B1);
    mptr_list.push_back(B2);
    mptr_list.push_back(C1);
    mptr_list.push_back(C2);
    return;
  }
  assert(maxwidth == n);
  Matrix<T> *A1 = new Matrix<T>(*A,V_SPLIT_LEFT); /* case 2c */
  Matrix<T> *A2 = new Matrix<T>(*A,V_SPLIT_RIGHT);
  Matrix<T> *B1 = new Matrix<T>(*B,H_SPLIT_UPPER);
  Matrix<T> *B2 = new Matrix<T>(*B,H_SPLIT_LOWER);
  matrix_multiplication_dc2(mtasks,mptr_list,wth,C, A1, B1);
  matrix_multiplication_dc2(mtasks,mptr_list,wth,C, A2, B2);
  mptr_list.push_back(A1);
  mptr_list.push_back(A2);
  mptr_list.push_back(B1);
  mptr_list.push_back(B2);
}

template <class T>
void matrix_multiplication_transposed(__attribute__ ((unused)) size_t thread_id,
                                      size_t task_num,
                                      void *thread_data)
{
  std::vector<MultiplicationTask<T>> *mtasks
    = static_cast<std::vector<MultiplicationTask<T>> *>(thread_data);

  auto mt = mtasks->at(task_num);
  matrix_multiplication_transposed<T>(std::get<0>(mt),
                                      *(std::get<1>(mt)),
                                      *(std::get<2>(mt)));
}

template <class T>
void matrix_multiplication_dc_threaded(size_t num_threads,
                                       size_t wth,
                                       Matrix<T> *C,
                                       const Matrix<T> *A,
                                       const Matrix<T> *B)
{
  std::vector<MultiplicationTask<T>> mtasks{};
  std::vector<Matrix <T> *> mptr_list{};
  matrix_multiplication_dc2(mtasks,mptr_list,wth,C,A,B);
  std::cout << "# number of multiplication tasks\t" << mtasks.size()
            << std::endl;
  if (num_threads == 1)
  {
    for (auto &&mt : mtasks)
    {
      matrix_multiplication_transposed<T>(std::get<0>(mt),
                                          *(std::get<1>(mt)),
                                          *(std::get<2>(mt)));
    }
  } else
  {
    pfn_run_threaded(num_threads,mtasks.size(),
                     matrix_multiplication_transposed<T>,(void *) &mtasks);
  }
  std::cout << "# number of matrices to delete\t" << mptr_list.size()
            << std::endl;
  for (auto &&mptr : mptr_list)
  {
    delete mptr;
  }
}
#endif

template <class T>
void matrix_multiplication_dc_2threads(Matrix<T> *C,
                                       const Matrix<T> *A,
                                       const Matrix<T> *B,
                                       size_t width_threshold)
{
  size_t m = A->rows();
  size_t n = A->columns();
  size_t l = B->columns();
  assert(n == B->rows() && m == C->rows() && l == C->columns());
  size_t maxwidth = std::max(m, std::max(n, l));
  if (maxwidth <= width_threshold) {
    matrix_multiplication_transposed(C, *A, *B);
    return;
  }
  if (maxwidth == m) /* case 2a */
  {
    Matrix<T> A1 = Matrix<T>(A,H_SPLIT_UPPER);
    Matrix<T> A2 = Matrix<T>(A,H_SPLIT_LOWER);
    Matrix<T> C1 = Matrix<T>(C,H_SPLIT_UPPER);
    Matrix<T> C2 = Matrix<T>(C,H_SPLIT_LOWER);
    std::thread thread1 = std::thread(matrix_multiplication_dc<T>,
                                      &C1,&A1,B,width_threshold);
    std::thread thread2 = std::thread(matrix_multiplication_dc<T>,
                                      &C2,&A2,B,width_threshold);
    thread1.join(); thread2.join();
    return;
  }
  if (maxwidth == l) /* case 2b */
  {
    Matrix<T> B1 = Matrix<T>(B,V_SPLIT_LEFT);
    Matrix<T> B2 = Matrix<T>(B,V_SPLIT_RIGHT);
    Matrix<T> C1 = Matrix<T>(C,V_SPLIT_LEFT);
    Matrix<T> C2 = Matrix<T>(C,V_SPLIT_RIGHT);
    std::thread thread1 = std::thread(matrix_multiplication_dc<T>,
                                      &C1,A,&B1,width_threshold);
    std::thread thread2 = std::thread(matrix_multiplication_dc<T>,
                                      &C2,A,&B2,width_threshold);
    thread1.join();
    thread2.join();
    return;
  }
  assert(maxwidth == n);
  Matrix<T> A1 = Matrix<T>(A,V_SPLIT_LEFT); /* case 2c */
  Matrix<T> A2 = Matrix<T>(A,V_SPLIT_RIGHT);
  Matrix<T> B1 = Matrix<T>(B,H_SPLIT_UPPER);
  Matrix<T> B2 = Matrix<T>(B,H_SPLIT_LOWER);
  Matrix<T> Ctmp = Matrix<T>(C->rows(),C->columns(),0);
  std::thread thread1 = std::thread(matrix_multiplication_dc<T>,
                                    C,&A1,&B1,width_threshold);
  std::thread thread2 = std::thread(matrix_multiplication_dc<T>,
                                    &Ctmp,&A2,&B2,width_threshold);
  thread1.join();
  thread2.join();
  (*C) += Ctmp;
} /* end of matrix_multiplication_dc_2threads */

template <class T>
void matrix_multiplication_dc_pow2_threads(int remain,
                                           Matrix<T> *C,
                                           const Matrix<T> *A,
                                           const Matrix<T> *B,
                                           size_t width_threshold)
{
  size_t m = A->rows();
  size_t n = A->columns();
  size_t l = B->columns();
  assert(n == B->rows() && m == C->rows() && l == C->columns());
  size_t maxwidth = std::max(m, std::max(n, l));
  if (maxwidth <= width_threshold) {
    matrix_multiplication_transposed(C, *A, *B);
    return;
  }
  if (maxwidth == m) /* case 2a */
  {
    Matrix<T> A1 = Matrix<T>(A,H_SPLIT_UPPER);
    Matrix<T> A2 = Matrix<T>(A,H_SPLIT_LOWER);
    Matrix<T> C1 = Matrix<T>(C,H_SPLIT_UPPER);
    Matrix<T> C2 = Matrix<T>(C,H_SPLIT_LOWER);
    if (remain > 0)
    {
      matrix_multiplication_dc_pow2_threads(remain - 1,
                                            &C1,&A1,B,width_threshold);
      matrix_multiplication_dc_pow2_threads(remain - 1,
                                            &C2,&A2,B,width_threshold);
    } else
    {
      std::thread thread1 = std::thread(matrix_multiplication_dc<T>,
                                        &C1,&A1,B,width_threshold);
      std::thread thread2 = std::thread(matrix_multiplication_dc<T>,
                                        &C2,&A2,B,width_threshold);
      thread1.join(); thread2.join();
    }
    return;
  }
  if (maxwidth == l) /* case 2b */
  {
    Matrix<T> B1 = Matrix<T>(B,V_SPLIT_LEFT);
    Matrix<T> B2 = Matrix<T>(B,V_SPLIT_RIGHT);
    Matrix<T> C1 = Matrix<T>(C,V_SPLIT_LEFT);
    Matrix<T> C2 = Matrix<T>(C,V_SPLIT_RIGHT);
    if (remain > 0)
    {
      matrix_multiplication_dc_pow2_threads(remain - 1,
                                            &C1,A,&B1,width_threshold);
      matrix_multiplication_dc_pow2_threads(remain - 1,
                                            &C2,A,&B2,width_threshold);
    } else
    {
      std::thread thread1 = std::thread(matrix_multiplication_dc<T>,
                                        &C1,A,&B1,width_threshold);
      std::thread thread2 = std::thread(matrix_multiplication_dc<T>,
                                        &C2,A,&B2,width_threshold);
      thread1.join();
      thread2.join();
    }
    return;
  }
  assert(maxwidth == n);
  Matrix<T> A1 = Matrix<T>(A,V_SPLIT_LEFT); /* case 2c */
  Matrix<T> A2 = Matrix<T>(A,V_SPLIT_RIGHT);
  Matrix<T> B1 = Matrix<T>(B,H_SPLIT_UPPER);
  Matrix<T> B2 = Matrix<T>(B,H_SPLIT_LOWER);
  if (remain > 0)
  {
    Matrix<T> Ctmp = Matrix<T>(C->rows(),C->columns(),0);
    matrix_multiplication_dc_pow2_threads(remain - 1,
                                          C,&A1,&B1,width_threshold);
    matrix_multiplication_dc_pow2_threads(remain - 1,
                                          &Ctmp,&A2,&B2,width_threshold);
    (*C) += Ctmp;
  } else
  {
    Matrix<T> Ctmp = Matrix<T>(C->rows(),C->columns(),0);
    std::thread thread1 = std::thread(matrix_multiplication_dc<T>,
                                      C,&A1,&B1,width_threshold);
    std::thread thread2 = std::thread(matrix_multiplication_dc<T>,
                                      &Ctmp,&A2,&B2,width_threshold);
    thread1.join();
    thread2.join();
    (*C) += Ctmp;
  }
}

template <class T>
static void cross_compare(const Matrix<T> *const *Ctab)
{
  unsigned int i = 0;
  const unsigned int last_op = matrix_mult_options_last_op();

  while (i < last_op)
  {
    if (Ctab[i] != nullptr)
    {
      unsigned int j = i + 1;

      while (j <= last_op)
      {
        if (Ctab[j] != nullptr)
        {
          if ((*Ctab[i]) != *(Ctab[j]))
          {
            Ctab[i]->show();
            Ctab[j]->show();
            std::cerr << "Matrix " << i << " and " << j << " differ"
                      << std::endl;
            exit(EXIT_FAILURE);
          }
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

template <class T>
static void matrixtab_delete(Matrix<T> **Ctab)
{
  unsigned int i;
  const unsigned int last_op = matrix_mult_options_last_op();

  for (i = 0; i <= last_op; i++)
  {
    if (Ctab[i] != nullptr)
    {
      delete Ctab[i];
    }
  }
  delete[] Ctab;
}

#define SHOW_RESULT_MATRIX(MATRIX)       \
  if (matrix_mult_options_show(options)) \
  {                                      \
    MATRIX->show();                      \
  }

template <class T>
static void matmulrun(const MMoptions *options)
{
  Matrix<T> *A, *B = nullptr;
  size_t rowsB = 0, columnsB = 0, rowsC = 0, columnsC = 0;
  const char *inputfileA, *inputfileB;
  const unsigned int last_op = matrix_mult_options_last_op();
  const T random_min = (T) 0,
          random_max = (T) 10;

  if (matrix_mult_options_showmatrixbasetype(options))
  {
    std::cout << matrix_mult_options_typestring(options) << std::endl;
    return;
  }
  matrix_mult_options_show_options(options);
  inputfileA = matrix_mult_options_inputfileA(options);
  if (inputfileA != nullptr)
  {
    A = new Matrix<T>(inputfileA);
  } else
  {
    A = new Matrix<T>(matrix_mult_options_rowsA(options),
                      matrix_mult_options_columnsA(options),0);
    if (matrix_mult_options_continuous(options))
    {
      A->init_continuous();
    } else
    {
      A->init_random(random_min, random_max);
    }
  }
  if (matrix_mult_options_show(options))
  {
    A->show();
  }
  inputfileB = matrix_mult_options_inputfileB(options);
  /* for splitting we only need matrix A. So we generate the other matrices
     only if any other operation is specified */
  if (!matrix_mult_options_only_split(options))
  {
    if (inputfileB != nullptr)
    {
      rowsC = A->rows();
      B = new Matrix<T>(inputfileB);
      rowsB = B->rows();
      columnsC = columnsB = B->columns();
    } else
    {
      rowsB = matrix_mult_options_columnsA(options);
      columnsC = columnsB = matrix_mult_options_columnsB(options);
      rowsC = matrix_mult_options_rowsA(options);
      B = new Matrix<T>(rowsB, columnsB,0);
      if (matrix_mult_options_continuous(options))
      {
        B->init_continuous();
      } else
      {
        B->init_random(random_min, random_max);
      }
    }
  }
  if (B != nullptr && matrix_mult_options_show(options))
  {
    B->show();
  }
  if (matrix_mult_options_split(options))
  {
    Matrix<T> Aupper = Matrix<T>(A,H_SPLIT_UPPER), /* h_split/upper */
              Alower = Matrix<T>(A,H_SPLIT_LOWER),/* h_split/lower */
              Aleft = Matrix<T>(A,V_SPLIT_LEFT), /* v_split/left */
              Aright = Matrix<T>(A,V_SPLIT_RIGHT);/* v_split/right */

    if (matrix_mult_options_show(options))
    {
      Aupper.show();
      Alower.show();
      Aleft.show();
      Aright.show();
    }
  }
  if (matrix_mult_options_addition(options))
  {
    assert(B != nullptr);
    Matrix<T> *C = *A + *B;
    SHOW_RESULT_MATRIX(C);
    delete C;
  }
  Matrix<T> **Ctab = new Matrix<T> *[last_op + 1];
  for (unsigned int idx = 0; idx <= last_op; idx++)
  {
    Ctab[idx] = nullptr;
  }
  if (matrix_mult_options_multiplication_standard(options))
  {
    Matrix<T> *C = *A * *B;
    Ctab[OP_multiplication_standard] = C;
    SHOW_RESULT_MATRIX(C);
  }
  if (matrix_mult_options_multiplication_transposed(options))
  {
    Matrix<T> *C = new Matrix<T>(rowsC, columnsC,0);
    Ctab[OP_multiplication_transposed] = C;
    matrix_multiplication_transposed<T>(C, *A, *B);
    SHOW_RESULT_MATRIX(C);
  }
  if (matrix_mult_options_multiplication_dc(options))
  {
    const unsigned long dc_width_threshold =
        matrix_mult_options_dc_width_threshold(options);
    Matrix<T> *C = new Matrix<T>(rowsC, columnsC,0);
    Ctab[OP_multiplication_dc] = C;
    const unsigned long num_threads = matrix_mult_options_num_threads(options);
    if (num_threads == 1)
    {
      matrix_multiplication_dc(C, A, B, dc_width_threshold);
    } else
    {
      if (num_threads == 2)
      {
        //matrix_multiplication_dc_2threads(C,A,B,dc_width_threshold);
        matrix_multiplication_dc_pow2_threads(1,C,A,B,dc_width_threshold);
      } else
      {
#ifdef NEWDCVERSION
        matrix_multiplication_dc_threaded(num_threads,
                                          dc_width_threshold,
                                          C,A,B);
#endif
        matrix_multiplication_dc_pow2_threads(2,C,A,B,dc_width_threshold);
      }
    }
    SHOW_RESULT_MATRIX(C);
  }
  delete A;
  delete B;
  cross_compare((const Matrix<T> *const *) Ctab);
  matrixtab_delete(Ctab);
}

int main(int argc, char *argv[])
{
  MMoptions *options = matrix_mult_options_new(argc, argv);
  const char *typestring;

  typestring = matrix_mult_options_typestring(options);
  if (strcmp(typestring,"double") == 0)
  {
    matmulrun<double>(options);
  } else
  {
    if (strcmp(typestring,"float") == 0)
    {
      matmulrun<float>(options);
    } else
    {
      assert (strcmp(typestring,"int") == 0);
      matmulrun<int>(options);
    }
  }
  matrix_mult_options_delete(options);
  return EXIT_SUCCESS;
}
