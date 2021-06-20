#include <iostream>
#include <cstdint>

template <typename mytype>
mytype getmax (mytype a, mytype b) {
  return a > b ? a : b;
}

int main(void)
{
  int32_t i = 5, j = 6, k;
  int64_t l = 10, m = 5, n;
  k = getmax<int32_t> (i,j);   
  /* <int32_t> is optional */
  n = getmax<int64_t> (l,m);
  /* <int64_t> is optional */
  std::cout << k << std::endl;
  std::cout << n << std::endl;
  return EXIT_SUCCESS;
}
