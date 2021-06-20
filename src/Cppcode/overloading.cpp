#include <cstring>
#include <iostream>

void print()
{
  std::cout << __func__ << "(no args)" << std::endl;
}

void print(int d, char *s)
{
  std::cout << "d=" << d << ", char *s=" << s << std::endl;
}

void print(int d, const char *s)
{
  std::cout << "d=" << d << ", const char *s=" << s << std::endl;
}

int print(float f)
{
  std::cout << "f=" << f << std::endl;
  return 0;
}

/*
int print(int d, char *s)
{
  std::cout << "d=" << d << ", const char *s=" << s << std::endl;
  return 0;
}
*/

int main(void)
{
  char *s = strdup("print with char *");

  print();
  print(3,s);
  print(3,"print with const char *");
  print(3.14159265358979);
}
