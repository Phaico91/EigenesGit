#include <stdio.h>

void inccbv(int i)   /* call by value */
{
  i++;
}
void inccbr(int *i)  /* call by reference via */
{                    /* call by value with int-ptr */
  (*i)++;
}
int main(void)
{
  int i = 0;
  inccbv(i);                         /* pass value i */
  printf("after inccbv: i=%d\n",i);  /* out: after inccbv: i=0 */
  inccbr(&i);                        /* pass address of i */
  printf("after inccbr: i=%d\n",i);  /* out: after inccbr: i=1 */
  return 0;
}
