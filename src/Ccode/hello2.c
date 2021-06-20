#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
  int x = 1235;
  int check = x > 0;
  printf("&x=%p\n",&x);
  printf("chaeck=%d\n",check);
  printf("chaeck=%s\n",!check ? "false" : "true");
  printf("Hello World!\n");
  return EXIT_SUCCESS;
}
