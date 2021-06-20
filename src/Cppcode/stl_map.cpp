#include <cstdlib>
#include <map>
#include <string>

using str_str_map = std::map<std::string,std::string>;

/* The rest of the file stems from
   http://www.cplusplus.com/reference/map/map/map/ */

bool fncomp (char lhs, char rhs)
{
  return lhs < rhs ? true : false;
}

struct classcomp
{
  bool operator() (const char& lhs, const char& rhs) const
  {
    return lhs < rhs ? true : false;
  }
};

int main(void)
{
  std::map<char,int> first;

  first['a']=10;
  first['b']=30;
  first['c']=50;
  first['d']=70;

  std::map<char,int> second (first.begin(),first.end());

  std::map<char,int> third (second);

  std::map<char,int,classcomp> fourth;                 // class as Compare

  std::map<char,int,bool(*)(char,char)> fifth (fncomp); // function as Compare
  return EXIT_SUCCESS;
}
