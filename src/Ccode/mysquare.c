#include <stdio.h>
int
mysquare1(int x)
{
  return x * x;
}
static int mysquare2(int x)
{
  return x * x;
}

int main(void)
{
  int x = 4;
  printf("mysquare2(%d)=%d\n",x,mysquare2(x));
  return 0;
}
