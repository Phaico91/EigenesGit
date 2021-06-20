#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc,char **argv)
{
  char **argvcopy = new char * [argc];

  for (int idx = 0; idx < argc; idx++)
  {
    argvcopy[idx] = strdup(argv[idx]); // implicit malloc
  }
  for (int idx = 0; idx < argc; idx++)
  {
    std::cout << "argv_copy[" << idx << "]="
              << argvcopy[idx] << std::endl;
  }
  for (int idx = 0; idx < argc; idx++)
  {
    free(argvcopy[idx]);
  }
  delete[] argvcopy;
  return EXIT_SUCCESS;
}
