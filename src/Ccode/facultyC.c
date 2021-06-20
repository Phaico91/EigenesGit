#include <stdio.h>
#include <stdlib.h>

size_t fac(size_t n) {
  if (n == 0) return 1;
  return n * fac(n-1);
}

int main(void) {
  printf("%d! = %lu\n",3,fac(3));
  return EXIT_SUCCESS;
}
