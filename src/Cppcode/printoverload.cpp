#include <iostream>
#include <cctype>
#include <cstring>

void print() { std::cout << "hello" << std::endl; }

void print(const char *s) { // okay, differs by argnum
  std::cout << __func__ << " with const: s=" << s << std::endl;
}

void print(char *s) { // okay, differs by ommitted const
  s[0] = toupper(s[0]);
  std::cout << __func__ << " without const: s=" << s << std::endl;
}

void print(float f) { // okay, differs in type
  std::cout << __func__ << " " << f << std::endl;
}

int main(void)
{
  char *s = strdup("hello");

  print();
  print((const char *) s);
  print(s);
  print(3.14);
  free(s);
  return EXIT_SUCCESS;
}
