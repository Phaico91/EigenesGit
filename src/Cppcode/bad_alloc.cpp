#include <cstdint>     // uint64_t
#include <cassert>     // assert
#include <iostream>    // std::cout
#include <new>         // std::bad_alloc
int main(int argc, const char *argv[]) {
  int exponent;
  if (argc != 2 || sscanf(argv[1],"%d",&exponent) != 1)
  {
    std::cerr << "Usage: " << argv[0] << " <exponent>" << std::endl; 
    return EXIT_FAILURE;
  }
  const uint64_t len = uint64_t(1) << exponent;
  double *arr = nullptr;
  try {
    arr = new double [len];
  }
  catch (std::bad_alloc& ba) {
    std::cerr << argv[0] << ": " << ba.what() << " exception caught"
              << std::endl;
    return EXIT_FAILURE;
  }
  uint64_t power_of_2 = 1;
  double sum = 0;

  for (uint64_t idx = 0; idx < len; idx++)
  {
    if (idx == power_of_2) {
      power_of_2 *= 2;
      std::cout << power_of_2 << std::endl;
    }
    arr[idx] = 3.14 * power_of_2;
    assert(idx <= power_of_2 && power_of_2 - idx < len);
    sum += arr[power_of_2 - idx];
  }
  delete[] arr; // special syntax for deleting arrays
  double size_GB = (len * sizeof *arr)/(1024.0 * 1024.0 * 1024.0);
  std::cout << "sizeof(arr)=" << size_GB << "Gb" << std::endl;
  std::cout << "sum = " << sum << std::endl;
  return EXIT_SUCCESS;
}
