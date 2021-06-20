#ifndef CIRCLE_HPP
#define CIRCLE_HPP
#include <cmath>
#include "shape.hpp"

template<typename T>
class Circle : public Shape {
 private: T radius;
 public:
  Circle(void)
    : Shape(),
      radius(1) { }
  Circle(T _radius)
    : Shape(),
      radius(_radius) { }
  Circle(const Circle& _circle)
    : Shape(),
      radius(_circle.radius) { }
  void radius_set(T _radius) { radius = _radius; }
  T radius_get(void) const { return radius; }
  double area_get(void) const {
    return M_PI * static_cast<double>(radius * radius);
  }
  std::string to_string(void) const {
    return "Circle, attr : {radius : " + std::to_string(radius) +
           ", area : " + std::to_string(area_get()) + "}";
  }
}; // end of class Circle
#endif
