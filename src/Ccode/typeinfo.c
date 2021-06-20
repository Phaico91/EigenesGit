#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

/*@unused@*/ __attribute__ ((unused)) static inline void formatbyte(
                                        char *buffer,unsigned long bs,
                                        int bits)
{
  int i;
  unsigned long mask = 1UL << (bits-1);

  for (i=0; i < bits; i++, mask >>= 1)
  {
    buffer[i] = (bs & mask) ? '1' : '0';
  }
  buffer[bits] = '\0';
}

#define SIGNED(TYPE)   (((TYPE) -1) < 0)
#define TYPEINFO(TYPE) {\
                         printf("%s is %ssigned %lu bits, ",#TYPE,\
                                 SIGNED(TYPE) ? "" : "un",\
                                 CHAR_BIT * sizeof (TYPE));\
                         if (SIGNED(TYPE))\
                         {\
                           TYPE minval = 1UL << (CHAR_BIT * sizeof (TYPE) - 1),\
                                maxval = minval - 1;\
                           printf("minval=%ld,maxval=%ld\n",\
                                   (long) minval,(long) maxval);\
                         } else\
                         {\
                           TYPE maxval = ~0;\
                           printf("minval=0,maxval=%lu\n",\
                                    (unsigned long) maxval);\
                         }\
                       }

int main(void)
{
  TYPEINFO(char);
  TYPEINFO(short);
  TYPEINFO(int);
  TYPEINFO(long);
  TYPEINFO(unsigned char);
  TYPEINFO(unsigned short);
  TYPEINFO(unsigned int);
  TYPEINFO(unsigned long);
  TYPEINFO(time_t);
  TYPEINFO(size_t);
  TYPEINFO(off_t);
  return EXIT_SUCCESS;
}
