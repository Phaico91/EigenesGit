#include <cstdlib>
#include <iostream>

class Mypair {
  private: int x, y;   // pair of integers to store values
  public: Mypair (int a,int b) {
            x = a; y = b;
          };
          Mypair operator+ (const Mypair &p) const {
            Mypair temp(x + p.x,y + p.y);
            return temp;
          }
          void show() const {
            std::cout << x << "," << y << std::endl;
          }
};
int main(void) {
  Mypair a (3,1); Mypair b (1,2); Mypair c = a + b;
  // c = a + b is equivalent to c = a.operator+ (b)
  c.show();
  return EXIT_SUCCESS;
}
