#ifndef LINE_HPP
#define LINE_HPP
#include <iostream>
#include <string>
#include "shape.hpp"

template<typename T>
class Line : public Shape {
 private: T length;
 public:
  Line(void)
    : Shape(),
      length(1) { }
  Line(T _length)
    : Shape(),
      length(_length) { }
  Line(const Line& _line)
    : Shape(),
      length(_line.length) { }
  void length_set(T _length) { length = _length; }
  T length_get(void) const { return length; }
  std::string to_string() const {
    return "Line, attr : {length : " + // concatenates strings
           std::to_string(length) + "}";
  }
}; // end of class Line
#endif
