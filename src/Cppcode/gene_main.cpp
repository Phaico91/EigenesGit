#include <cstdlib>
#include <iostream>
#include "gene.hpp"
#include "geneinter.hpp"

int main(void)
{
  Gene pccx1 ("PCCX1");
  pccx1.length_set(1460);
  Gene *mmp11 = new Gene("MMP11");
  mmp11->length_set(2260);
  delete mmp11;
  Gene genelist[4] = {
    Gene("MMP3"),Gene("ZNF148")
  };
  Gene *table = new Gene [4];
  table[0].length_set(1024);
  table[1].length_set(2048);
  for (size_t idx = 0; idx < 2; idx++)
  {
    std::cout << "name=" << genelist[idx].name_get() << "\t";
    std::cout << "len=" << table[idx].length_get() << std::endl;
  }
  delete [] table;
  return EXIT_SUCCESS;
}
