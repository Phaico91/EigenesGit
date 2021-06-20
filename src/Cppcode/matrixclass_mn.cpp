#include <cstdlib>
#include "matrixclass.hpp"

template<typename T>
void matrix_example(size_t m,
                    size_t n,
                    size_t k,
                    T min,T max)
{
  Matrix<T> A = Matrix<T>(m,n);
  Matrix<T> B = Matrix<T>(n,k);
  A.init_random(min,max);
  B.init_random(min,max);
  Matrix<T> *C = A * B;
  // implementation not shown
  C->show(); 
  delete C;
}

int main(void)
{
  matrix_example<double>(4,4,4,
                         0.0,1.0);
  matrix_example<int>(4,4,4,0,10);
  return EXIT_SUCCESS;
}
