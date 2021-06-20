#include <iostream>

int val = 4711;   // global variable

int main(void)
{
  int val = 1234; // local variable
  std::cout << "value of global var.: " << ::val << std::endl;
  std::cout << "value of local var.: " << val << std::endl;
}
