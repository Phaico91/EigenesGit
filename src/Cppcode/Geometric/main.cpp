#include "shape.hpp"
#include "line.hpp"
#include "circle.hpp"
#include "rectangle.hpp"
#include "square.hpp"

template<typename T>
static void output(const char *name,const T &obj) {
  std::cout << name << " : {id : " << obj.iid()
            << ", class : " << obj.to_string()
            << "}" << std::endl;
}

int main(void)
{
  Shape sh;
  Line<int> l0{}, l1{3}, l2{l1};
  Circle<int> c0{}, c1{2}, c2{c0};
  Rectangle<int> r0{}, r1{1, 2}, r2{l1, l2}, r3{r0};
  Square<int> sq0{}, sq1{10}, sq2{l1}, sq3{sq2};
  output("sh",sh);
  output("l0",l0);
  output("l1",l1);
  output("l2",l2);
  l2.length_set(10);
  output("l2",l2);
  output("c0",c0);
  output("c1",c1);
  output("c2",c2);
  output("r0",r0);
  output("r1",r1);
  output("r2",r2);
  output("r3",r3);
  output("sq0",sq0);
  output("sq1",sq1);
  sq1.side_length_set(5);
  output("sq1",sq1);
  output("sq2",sq2);
  output("sq3",sq3);
  Rectangle r4(2.5,2.8);
  output("r4",r4);
  return EXIT_SUCCESS;
}
