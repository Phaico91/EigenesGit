#include <iostream>

void print(int i = 0,int j = 1,int k = 2) {
  std::cout << i << " " << j 
            << " " << k << std::endl;
}

int main(void) {
  print();
  print(7);
  print(5,6);
  print(7,8,9);
}
