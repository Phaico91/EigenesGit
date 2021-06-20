#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>

typedef long double LongDouble;
#define DECL_MASCHINEACC(TYPE,EPSILON)\
static void maschine_accurracy_##TYPE(const char *format)\
{\
  unsigned long iterations;\
  TYPE myeps;\
  for (iterations = 0, myeps = (TYPE) 1.0;\
       (TYPE) 1.0 + myeps > (TYPE) 1.0;\
       iterations++, myeps/=(TYPE) 2.0)\
          /* Nothing */;\
  printf("iterations=%lu, predefined=",iterations);\
  printf(format,EPSILON/2.0);\
  printf(", myeps=");\
  printf(format,myeps);\
  printf("\n");\
}

DECL_MASCHINEACC(float,FLT_EPSILON)
DECL_MASCHINEACC(double,DBL_EPSILON)
DECL_MASCHINEACC(LongDouble,LDBL_EPSILON)

int main(void)
{
  printf("sizeof(float)=%d,sizeof(double)=%d\n",(int) sizeof (float),
                                                (int) sizeof (double));
  printf("FLT_MIN=%e\n",FLT_MIN);
  printf("FLT_MAX=%e\n",FLT_MAX);
  printf("DBL_MIN=%e\n",DBL_MIN);
  printf("DBL_MAX=%e\n",DBL_MAX);
  maschine_accurracy_float("%e");
  maschine_accurracy_double("%e");
  maschine_accurracy_LongDouble("%Le");
  return EXIT_SUCCESS;
}
