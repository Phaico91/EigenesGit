#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

template <class T>
class Matrix
{
 private:
  T * *M;             /* array of row pointers */
  size_t n_rows,      /* number of rows */
         n_cols;      /* number of columns */
  size_t row_start,   /* first row of submatrix */
         col_start,   /* first column of submatrix */
         origcols;    /* refers to original matrix */
  bool hasownvalues,  /* has matrix own value? */
       initialized;   /* are matrix values initialed? */

  void create_empty_matrix(size_t _n_rows, size_t _n_cols)
  {
    assert(_n_rows > 0 && _n_cols > 0);
    hasownvalues = true;
    initialized = false;
    n_rows = _n_rows;
    n_cols = origcols = _n_cols;
    row_start = col_start = 0;
    M = new T * [n_rows]; // allocate array of row pointers
    M[0] = new T[n_rows * n_cols];  // array of values
    for (size_t r = 1; r < n_rows; r++)
      M[r] = M[r - 1] + n_cols;  // set row pointer
  }

 public:
  Matrix(size_t _n_rows, size_t _n_cols)
  {  // constructor
    create_empty_matrix(_n_rows, _n_cols);
  }

  Matrix(size_t _n_rows, size_t _n_cols, T value)
  { // constructor with inititialiation
    create_empty_matrix(_n_rows, _n_cols);
    memset(M[0], value, n_rows * n_cols * sizeof(T));
    initialized = true;
  }

  Matrix(const char *filename)
  { // constructor reading matrix from file
    /* Phase 1 */
    std::ifstream infile_matrix(filename);

    if (infile_matrix.fail())
    {
      std::cerr << "couldn't open file " << filename << std::endl;
      exit(EXIT_FAILURE);
    }
    /* Phase 2 */
    std::vector<T> values;
    std::string line;
    size_t count_rows = 0, count_columns = 0;
    while (getline(infile_matrix, line))
    {
      std::stringstream linestream(line);
      T value;
      size_t current_column = 0;
      while (linestream >> value)
      {
        values.push_back(value);
        current_column++;
      }
      if (count_rows == 0)
      {
        count_columns = current_column;
      } else
      {
        assert(count_columns == current_column);
      }
      count_rows++;
    }
    /* Phase 3 */
    create_empty_matrix(count_rows, count_columns);
    size_t idx = 0;
    for (size_t r = 0; r < count_rows; r++)
    {
      for (size_t c = 0; c < count_columns; c++)
      {
        M[r][c] = values[idx++];
      }
    }
    initialized = true;
  }

  Matrix(const Matrix *parent, bool h_split, bool first) // first=upper/left
  { // constructor for views on matrices expressed by row pointers
    T *firstsubrow = nullptr;
    origcols = parent->origcols;
    if (h_split)
    {
      if (first)
      {
        n_cols = parent->n_cols;
        col_start = parent->col_start;
        n_rows = parent->n_rows / 2 +
                 (parent->n_rows % 2);
        row_start = parent->row_start;
        firstsubrow = parent->M[0];
      } else
      {
        n_cols = parent->n_cols;
        col_start = parent->col_start;
        n_rows = parent->n_rows / 2;
        size_t firstrows = parent->n_rows - n_rows;
        row_start = parent->row_start + firstrows;
        firstsubrow = parent->M[firstrows];
      }
    } else
    {
      if (first)
      {
        n_rows = parent->n_rows;
        row_start = parent->row_start;
        n_cols = parent->n_cols / 2 +
                 (parent->n_cols % 2);
        col_start = parent->col_start;
        firstsubrow = parent->M[0];
      } else
      {
        n_rows = parent->n_rows;
        row_start = parent->row_start;
        n_cols = parent->n_cols / 2;
        size_t fstcols = parent->n_cols - n_cols;
        col_start = parent->col_start + fstcols;
        firstsubrow = parent->M[0] + fstcols;
      }
    }
    hasownvalues = false;
    initialized = parent->initialized;
    M = new T *[n_rows];
    M[0] = firstsubrow;
    for (size_t r = 1; r < n_rows; r++)
      M[r] = M[r-1] + parent->origcols; // consec. rows at constant distance
  }

  ~Matrix()
  {  // destructor
    if (hasownvalues) delete[] M[0];
    delete[] M;
  }

  size_t rows(void) const { return n_rows; }
  size_t columns(void) const { return n_cols; }

  T operator()(size_t r, size_t c) const {  // M(r,c)
    assert(initialized);
    return M[r][c];
  }

  T get(size_t r, size_t c) const {
    assert(initialized);
    return M[r][c];
  }

  void set(size_t r, size_t c, T v) { M[r][c] = v; }

  Matrix<T> *transpose(void) const
  {
    assert(initialized);
    Matrix<T> *AT = new Matrix<T>(n_cols, n_rows);
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
        AT->set(c, r, M[r][c]);
      }
    }
    AT->initialized = true;
    return AT;
  }

  T total() const
  {
    T sum = 0;
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
        sum += M[r][c];
      }
    }
    return sum;
  }

  void show() const
  {
    T absminvalue, absmaxvalue, minvalue = 0, maxvalue = 0;
    bool minvalue_defined = false, maxvalue_defined = false;
    int width;

    assert(initialized);
    std::cout << n_rows << " X " << n_cols << std::endl;
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
        if (!minvalue_defined || M[r][c] < minvalue)
        {
          minvalue_defined = true;
          minvalue = M[r][c];
        }
        if (!maxvalue_defined || M[r][c] > maxvalue)
        {
          maxvalue_defined = true;
          maxvalue = M[r][c];
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
    if (std::is_floating_point<T>::value)
    {
      width += 3;
    }
    if (minvalue < 0 || maxvalue < 0)
    {
      width++;
    }
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
        if constexpr (std::is_floating_point<T>::value)
        {
          std::cout.width(2);
          std::cout << std::setprecision(width) << M[r][c];
        } else
        {
          std::cout.width(width);
          std::cout << std::right << M[r][c];
        }
      }
      std::cout << std::endl;
    }
  }

  Matrix<T> *operator+(const Matrix<T> &B) const
  {
    assert(initialized && B.initialized &&
           n_cols == B.columns() && n_rows == B.rows());
    Matrix<T> *C = new Matrix<T> (n_rows, n_cols);
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
	C->set(r, c, M[r][c] + B(r,c));
      }
    }
    C->initialized = true;
    return C;
  }

  Matrix<T> *operator*(const Matrix<T> &B) const
  {
    assert(initialized && B.initialized &&
           n_cols == B.rows());
    Matrix<T> *C = new Matrix<T> (n_rows,B.columns());
    for (size_t i = 0; i < C->rows(); i++) {
      for (size_t j = 0; j < C->columns(); j++) {
	T sum = 0;
	for (size_t k = 0; k < n_cols; k++) {
	  sum += M[i][k] * B(k, j);
        }
	C->set(i, j, sum);
      }
    }
    C->initialized = true;
    return C;
  }

  void operator +=(const Matrix<T> &B)
  {
    assert(initialized && B.initialized &&
           n_cols == B.columns() && n_rows == B.rows());
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
	M[r][c] += B(r,c);
      }
    }
  }

  bool operator==(const Matrix<T> &B) const
  {
    assert(initialized && B.initialized &&
           n_rows == B.rows() && n_cols == B.columns());
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
        if constexpr (std::is_floating_point<T>::value)
        {
          const double tol = 1e-10;
          if (fabs(M[r][c] - B(r, c)) > tol) return false;
        } else
        {
	  if (M[r][c] != B(r, c)) return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const Matrix<T> &B) const
  {
    return *this == B ? false : true;
  }

  void init_random(T min, T max)
  {
    assert(min <= max);
    double rng = static_cast<double>(max - min + 1);
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (size_t r = 0; r < n_rows; r++) {
      for (size_t c = 0; c < n_cols; c++) {
        double rv = dist(engine) * rng;
        M[r][c] = static_cast<T>(static_cast<double>(min)+rv);
      }
    }
    initialized = true;
  }

  void init_continuous(void)
  {
    T minvalue = 0;
    for (size_t r = 0; r < n_rows; r++)
    {
      for (size_t c = 0; c < n_cols; c++)
      {
        M[r][c] = minvalue++;
      }
    }
    initialized = true;
  }

};
