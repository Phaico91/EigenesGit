#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
/* Code does not compile, as print is ambiguous. So there is not
   corresponding goal to compile it. That is intended. */

void print(char *string)
{
  std::cout << "print with one argument: " << string << std::endl;
}

void print(char *string,
           int nl = 1)
{
  std::cout << "print with one default argument: " << string 
            << ", nl=" << nl << std::endl;
}

int main(void)
{
print("Test"); 
// which function to call?
  exit(EXIT_SUCCESS);
}
