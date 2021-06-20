#include <stdlib.h>
#include <stdio.h>
void *fun(void)
{
  printf("the big void\n");
  return NULL;
}
int main(void)
{
  (void) fun();
  return EXIT_SUCCESS;
}
