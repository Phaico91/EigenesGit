#include <iostream>       // std::cout
#include <thread>         // std::thread

void f(int x) {
  std::cout << "call f(" << x << ")" 
            << std::endl;
}

int main(void) {
  std::thread t (f,3);// call f(3) in thread:
  std::cout << "main and f now exec. "
               "concurrently" << std::endl;
  t.join(); // join thread t
  std::cout << "f completed." << std::endl;
  return EXIT_SUCCESS;
}
