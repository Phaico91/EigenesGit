#ifndef SQUARE_HPP
#define SQUARE_HPP
#include "rectangle.hpp"
#include "line.hpp"

template<typename T>
class Square : public Rectangle<T>
{
 public:
  Square(void) : Rectangle<T>(1, 1) { }
  Square(T _side_length)
    : Rectangle<T>(_side_length, _side_length) { }
  Square(const Line<T>& _line)
    : Rectangle<T>(_line.length_get(), 
                   _line.length_get()) { }
  Square(const Square& _square)
    : Rectangle<T>(_square.height, _square.width) { }
  void side_length_set(T _side_length) {
    Rectangle<T>::height = _side_length;
    Rectangle<T>::width = _side_length;
  }
  T side_length_get(void) const {
    return Rectangle<T>::height_get();
  }
  T area_get(void) const {
    return Rectangle<T>::area_get();
  }
  std::string to_string(void) const {
    return "Square, attr : {side_length : " +
           std::to_string(side_length_get()) +
           ", area : " + std::to_string(area_get()) + "}";
  }
}; // end of class Square
#endif
