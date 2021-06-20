#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <vector>
#include <cassert>

class Intarray
{
  int *array;
  uint64_t len;
  public:
  Intarray () {}; // constructor without initialization
  Intarray (const int *,uint64_t); // constr. w. init.
  ~Intarray (); // destructor
  bool lowerthan(const Intarray *param) const;
  void show () const;
};


Intarray::Intarray (const int *arr, uint64_t l)
{
  this->array = new int [l];
  for (uint64_t idx = 0; idx < l; idx++)
    this->array[idx] = arr[idx];
  this->len = l;
}

Intarray::~Intarray ()
{
  delete[] this->array;
}

bool Intarray::lowerthan(const Intarray *param) const
{ // last const => this function does not change class vars
  const int *arr_a = this->array;
  uint64_t len_a = this->len;
  const int *arr_b = param->array;
  uint64_t len_b = param->len;

  for (uint64_t idx = 0; /* Nothing */; idx++)
  {
    if (idx < len_a)
    {
      if (idx < len_b)
      {
        if (arr_a[idx] != arr_b[idx])
          return arr_a[idx] < arr_b[idx] ? true : false;
      } else
      {
        return false; // arr_b is proper prefix of arr_a
      }
    } else
    {
      return true; // arr_a is proper prefix of arr_b
    }
  }
  return false; // arr_a == arr_b
}

void Intarray::show () const
{
  std::cout << "vector = [";
  for (uint64_t v = 0; v < this->len; v++)
  {
    std::cout << this->array[v]
              << (v < this->len - 1 ? "," : "]") << std::endl;
  }
}

bool intarray_lowerthan(const Intarray *a,const Intarray *b)
{
  return a->lowerthan(b);
}

typedef std::vector<Intarray *> IA_tab;

int main(int argc,char *argv[])
{
  const uint32_t minlen = 3U;
  const uint32_t maxlen = 10U;
  const int minvalue = 1;
  const int maxvalue = 4;
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " num of vectors" << std::endl;
    exit(EXIT_FAILURE);
  }
  int numofvectors;
  if (sscanf(argv[1],"%d",&numofvectors) != 1 ||
      numofvectors <= 1)
  {
    std::cerr << "Usage: " << argv[0] << 
                 " num of vectors must be >= 2" << std::endl;
    exit(EXIT_FAILURE);
  }
  srand48(366292341);
  IA_tab ia_tab;
  int *arr = new int [maxlen+1];

  for (int idx = 0; idx < numofvectors; idx++)
  {
    uint32_t len = minlen + drand48() *
                   (maxlen - minlen + 1);
    assert(minlen <= len && len <= maxlen);
    for (uint64_t v = 0; v < len; v++)
    {
      arr[v] = minvalue +
               drand48() * (maxvalue - minvalue + 1);
      assert(minvalue <= arr[v] && arr[v] <= maxvalue);
    }
    Intarray *vec = new Intarray (arr,len);
    ia_tab.push_back(vec);
  }
  delete[] arr;
  std::sort(ia_tab.begin(),ia_tab.end(),intarray_lowerthan);



  
  Intarray *previous = nullptr;
  for (auto it = ia_tab.begin(); it != ia_tab.end(); it++)
  {
    if (previous != nullptr && !previous->lowerthan(*it))
    {
      previous->show();
      std::cerr << ">=";
      (*it)->show();
      exit(EXIT_FAILURE);
    }
    previous = *it;
    (*it)->show();
  }
  for (auto it = ia_tab.begin(); it != ia_tab.end(); it++)
    delete *it;
  return EXIT_SUCCESS;
}
