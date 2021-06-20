#include <iostream>
#include <iomanip>

template <typename T>
T accuracy(void) {
  T f, eps = (T) 1.0;
  for (f = (T) 1.0; (T) 1.0 + f > (T) 1.0; f /= (T) 2.0)
     eps = f;
  return eps;
}

int main(void) {
  float f = accuracy<float>();
  double d = accuracy<double>();
  long double ld = accuracy<long double>();
  std::cout << "accuracy float=" << std::setprecision(7)
                                 << f << std::endl;
  std::cout << "accuracy double=" << std::setprecision(7)
                                  << d << std::endl;
  std::cout << "accuracy long double=" << std::setprecision(7)
                                       << ld << std::endl;
}
