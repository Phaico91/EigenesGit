#ifndef MATRIXMULT_H
#define MATRIXMULT_H
#include <stddef.h>
#include <stdbool.h>

typedef struct Matrix Matrix;  /* from matrixmult.h */

size_t Matrixbasetype_sizeof(void);
const char *Matrixbasetype_string(void);
Matrix *matrix_new(size_t rows,size_t columns);
Matrix *matrix_new_read(const char *filename);
Matrix *matrix_new_random(size_t rows,size_t columns);
Matrix *matrix_new_continuous(size_t rows,size_t columns);
size_t matrix_rows(const Matrix *matrix);
size_t matrix_columns(const Matrix *matrix);
void matrix_show(const Matrix *matrix);
void matrix_compare(const Matrix *A,const Matrix *B);
Matrix *matrix_h_split_upper(const Matrix *parent);
Matrix *matrix_h_split_lower(const Matrix *parent);
Matrix *matrix_v_split_left(const Matrix *parent);
Matrix *matrix_v_split_right(const Matrix *parent);
void matrix_delete(Matrix *matrix);
void matrix_addition(Matrix *C,const Matrix *A,const Matrix *B);
void matrix_addition_inplace(Matrix *A,const Matrix *B);
void matrix_multiplication(Matrix *C,const Matrix *A,const Matrix *B);
void matrix_multiplication_dc(int level,Matrix *C,
                              const Matrix *A,const Matrix *B,
                              size_t widththreshold);

void matrix_multiplication_dc_iter(Matrix *C,
                                   const Matrix *A,const Matrix *B,
                                   size_t widththreshold);

void matrix_multiplication_dc_2threads(int level,Matrix *C,const Matrix *A,
                                       const Matrix *B,
                                       size_t widththreshold);
size_t recursive_calls_dc(void);
size_t directmultiplications_dc(void);

void matrix_multiplication_transposed(Matrix *C,
                                      const Matrix *A,
                                      const Matrix *B);
                                      
void matrix_multiplication_vectorized(Matrix *C,const Matrix *A,
                                      const Matrix *B);

void matrix_multiplication_transposed_vectorized(Matrix *C,const Matrix *A,
                                                 const Matrix *B);
#endif
