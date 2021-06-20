#ifndef SHAPE_HPP
#define SHAPE_HPP
#include <cstddef>
#include <iostream>
#include <string>

class Shape
{
 private:
  // class var
  static size_t instance_counter;
  size_t instance_id; // member var

 public:
  Shape(void)
    : instance_id(instance_counter) {
    instance_counter++;
  }
  size_t iid(void) const {
    return instance_id;
  }
  std::string to_string(void) const {
    return "Shape";
  }
}; // end of class Shape
size_t Shape::instance_counter{0};

#endif
