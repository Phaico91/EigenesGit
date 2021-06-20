#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "shape.hpp"
#include "line.hpp"

template<typename T>
class Rectangle : public Shape
{
 protected: // so subclasses can acces them
  T height, width;

 public:
  Rectangle(void)
      : Shape(),
        height(1),
        width(1) { }
  Rectangle(T _height, T _width)
      : Shape(),
        height(_height),
        width(_width) { }
  Rectangle(const Line<T> &_l1, const Line<T> &_l2)
      : Shape(),
        height(_l1.length_get()),
        width(_l2.length_get()) { }
  Rectangle(const Rectangle<T>& _rectangle)
      : Shape(),
        height(_rectangle.height),
        width(_rectangle.width) { }
  void height_set(T _height) { height = _height; }
  void width_set(T _width) { width = _width; }
  T height_get(void) const { return height; }
  T width_get(void) const { return width; }
  T area_get(void) const { return height * width; }
  std::string to_string() const {
    return "Rectangle, attr : {height : " +
           std::to_string(height) +
           ", width : " + std::to_string(width) +
           ", area : " + std::to_string(area_get()) + "}";
  }
}; // end of class Rectangle
#endif
