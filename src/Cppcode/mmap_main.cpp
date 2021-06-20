#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "mmap.hpp"

int main(int argc,char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return EXIT_FAILURE;
  }
  Mappedfile *mpf = new Mappedfile(argv[1]);
  std::cout << mpf->filename_get() << std::endl;
  std::cout << "characters=" << mpf->filesize_get()
                             << std::endl;
  std::cout << "lines=" << mpf->numberoflines_get()
                        << std::endl;
  std::cout << "words=" << mpf->numberofwords_get()
                        << std::endl;
  delete mpf;
  return EXIT_SUCCESS;
}
