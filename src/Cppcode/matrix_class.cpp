#include <cstdlib>
#include <cstdint>

template <typename T>
class Matrix
{
  private:
    uint64_t n_rows, n_cols;
    T **matrix;
  public:
    Matrix(uint64_t rows,uint64_t cols)
    {
      n_rows = rows;
      n_cols = cols;
      T **matrix = new T* [rows];
      matrix[0] = new T [rows * cols];
      for (uint64_t idx = 1; idx < rows; idx++)
      {
        matrix[idx] = matrix[idx-1] + cols;
      }
    }
};
