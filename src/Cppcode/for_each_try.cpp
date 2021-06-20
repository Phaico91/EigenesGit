//#include <execution> // does not seem to work
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
static void show_vector(const std::vector<T> &vec)
{
  for (auto v : vec)
  {
    std::cout << v << std::endl;
  }
}

static void square(size_t &val)
{
  val = val * val;
}
 
int main(void)
{
  std::vector<size_t> vec;
  
  for (size_t idx = 0; idx < 20; idx++)
    vec.push_back(idx);

  std::for_each(//std::execution::par, does not seem to work
                vec.begin(), vec.end(), square);
  show_vector(vec);
}
