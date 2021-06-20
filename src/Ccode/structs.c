#include <stdio.h>

typedef struct {
  unsigned int negative:1;
  unsigned int exponent:8;
  unsigned int mantissa:23;
} IEEEfloat; 

typedef struct
{ /* requires 3 * 4 bytes */
  unsigned int black:1,
               value:31,
               left,
               right;
} Redblacktreenode;

int main(void)
{
  struct {
    char x;
    int y; /* address % 4 == 0 */
    char z;
  } s1;

  printf("sizeof s1=%lu "
         "bytes\n",sizeof s1);
         /*sizeof s1=12 bytes*/

  struct {
    char x, z;
    int y;
  } s2;

  printf("sizeof s2=%lu "
         "bytes\n",
         sizeof s2);
        /*sizeof s2=8 bytes*/

  struct {
    char x; char pad_x[3];
    int y;
    char z; char pad_z[3];
  } s1_pad;

  printf("sizeof s1_pad=%lu bytes\n",
         sizeof s1_pad);
         /*sizeof s1_pad=12 bytes*/

  struct {
    char x, z; char pad_xz[2];
    int y;
  } s2_pad;

  printf("sizeof s2_pad=%lu bytes\n",
         sizeof s2_pad);
  /*sizeof s2_pad=8 bytes*/

  struct person {
    char *name;
    int age;
    float height;
    struct { /* embedded structure */
      int month;
      int day;
      int year;
    } birth;
  };

  struct person me, class[60];
  me.birth.year = 1977;
  /* array of info about everyone in class */
  class[0].name = "Smith"; class[0].birth.year = 1971;

  return (me.birth.year > class[0].birth.year) ? 0 : 1;
}
