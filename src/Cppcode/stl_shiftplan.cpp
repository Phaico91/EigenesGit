#include <algorithm>  // for find
#include <iostream>   // for cout
#include <vector>
#include <string>

using str_vec = std::vector<std::string>;

int main(void) {
  str_vec shpl; // the shiftplan
  std::cout << "# shifts = " << shpl.size() << std::endl; // 0
  shpl.push_back("Peter");
  shpl.push_back("Paul");
  shpl.push_back("Mary");
  shpl.push_back("Peter");
  shpl.push_back("John");
  std::cout << "num_shifts=" << shpl.size() << std::endl; // 5
  // John should take the second shift */
  shpl.insert(shpl.begin() + 1,"John");
  // Peter has done too much => no second shift for him
  shpl.erase(std::find(shpl.begin()+1,shpl.end(),"Peter"));
  for (auto it = shpl.begin(); it != shpl.end(); it++)
  {
    std::cout << *it << std::endl;
  }
}
