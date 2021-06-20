#include <cstdlib>
#include <iostream>
int main(void) {
  int readint; // I/O variable
  std::cin >> readint;// note the different comment syntax
  std::cout << "readint=" << readint << std::endl;
  return EXIT_SUCCESS;
}
