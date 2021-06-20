#include <iostream>

int global = 0;
int &func(void) {
  return global; // returns reference to global
}

int main(void) {
  int x = func() + 1; // x = global + 1
  func() = 3;     // global = 3
  std::cout << "x=" << x << ", global=" << global << std::endl;
}
