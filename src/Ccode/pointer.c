#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  int a, b; int *c; int *d;
  /* *d = 17; BAD */ d = &b; *d = 17;
  a = 2; b = 3; c = &a; d = &b;
  printf("a=%d,b=%d\n",a,b);
  printf("c=%p,d=%p,d-c=%ld\n",c,d,d-c);
  if (*c == *d) printf("Same value\n"); /* no */
  *c = 3;
  if (*c == *d) printf("Now same value\n"); /* yes */
  c = d;
  printf("c=%p,d=%p\n",c,d);
  if (c == d) printf ("Now same address\n"); /* yes */
  int vec[20];
  printf("vec=%p,&vec[0]=%p\n",vec,&vec[0]);
  return 0;
}
