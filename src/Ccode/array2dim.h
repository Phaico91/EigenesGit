#ifndef ARRAY2DIM_H
#define ARRAY2DIM_H
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
/* Allocates a 2-dim. array with dimensions <ROWS> x <COLS> and
   assigns a pointer to the newly allocated space to <ARR2DIM>.
   Size of each element is determined from type of the <ARR2DIM>
   pointer. <TYPE> is used for typecast, which is required in C++.
   Elements in array are initialized with 0. */
#define array2dim_malloc(ARR2DIM,TYPE,ROWS, COLS) {\
          size_t idx;\
          ARR2DIM = (TYPE **) malloc((ROWS) * sizeof (*ARR2DIM));\
          assert((ARR2DIM) != NULL);\
          (ARR2DIM)[0] = (TYPE *) calloc((ROWS) * (COLS),\
                                         sizeof (**ARR2DIM));\
          assert((ARR2DIM)[0] != NULL);\
          for (idx = 1UL; idx < (ROWS); idx++) \
            (ARR2DIM)[idx] = (ARR2DIM)[idx-1] + (COLS);\
        }

#define array2dim_delete(ARR2DIM)\
        if ((ARR2DIM) != NULL) {\
          free((ARR2DIM)[0]);\
          free(ARR2DIM);\
        }
#endif
