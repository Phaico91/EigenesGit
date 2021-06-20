#ifndef MATRIXMULT_OPT_H
#include <stdbool.h>
typedef struct MMoptions MMoptions;

typedef enum
{
  OP_multiplication_standard,
  OP_multiplication_transposed,
  OP_multiplication_dc,
  OP_multiplication_vector,
  OP_multiplication_transposed_vector, /* must be last of multiplication ops */
  OP_addition,
  OP_split /* must be last */
} Matrixoperation;

MMoptions *matrix_mult_options_new(int argc,char * const argv[]);
void matrix_mult_options_delete(MMoptions *options);
void matrix_mult_options_show_options(const MMoptions *options);
unsigned int matrix_mult_options_last_op(void);
bool matrix_mult_options_showmatrixbasetype(const MMoptions *options);
const char *matrix_mult_options_inputfileA(const MMoptions *options);
const char *matrix_mult_options_inputfileB(const MMoptions *options);
bool matrix_mult_options_continuous(const MMoptions *options);
unsigned long matrix_mult_options_rowsA(const MMoptions *options);
unsigned long matrix_mult_options_columnsA(const MMoptions *options);
unsigned long matrix_mult_options_columnsB(const MMoptions *options);
bool matrix_mult_options_show(const MMoptions *options);
unsigned long matrix_mult_options_num_threads(const MMoptions *options);
unsigned long matrix_mult_options_dc_width_threshold(const MMoptions *options);
bool matrix_mult_options_only_split(const MMoptions *options);
bool matrix_mult_options_split(const MMoptions *options);
bool matrix_mult_options_addition(const MMoptions *options);

bool matrix_mult_options_multiplication_standard(const MMoptions *options);
bool matrix_mult_options_multiplication_transposed(const MMoptions *options);
bool matrix_mult_options_multiplication_dc(const MMoptions *options);
bool matrix_mult_options_multiplication_vector(const MMoptions *options);
bool matrix_mult_options_multiplication_transposed_vector(
                      const MMoptions *options);
#ifdef __cplusplus
const char *matrix_mult_options_typestring(const MMoptions *options);
#endif

#endif
