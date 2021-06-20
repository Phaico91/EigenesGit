#include <cstdlib>
#include <cstring>
/* Allocates a 2-dimensional array with dimensions <ROWS> x <COLS>
   and assigns a pointer to the newly allocated space to <ARR2DIM>.
   Size of each element is determined from parameter TYPE.
   Elements in array are initialized with 0. */
#define array2dim_malloc(ARR2DIM, TYPE, ROWS, COLS)\
        {\
          ARR2DIM = new TYPE* [ROWS];\
          (ARR2DIM)[0] = new TYPE [(ROWS) * (COLS)];\
          memset(ARR2DIM[0],0,(ROWS) * (COLS) * sizeof(TYPE));\
          for (unsigned long _row = 1; _row < (ROWS); _row++) \
            (ARR2DIM)[_row] = (ARR2DIM)[_row-1] + (COLS);\
        }

#define array2dim_delete(ARR2DIM)\
        if ((ARR2DIM) != NULL)\
        {\
          delete [] (ARR2DIM)[0];\
          delete [] (ARR2DIM);\
        }
