#ifndef GENEINTER_H
#define GENEINTER_H
#include <cstdbool>
#include "gene.hpp"

class GeneInteraction {
  private:
    bool supresses = false, enhances = false;
    Gene g1, *g2;
  public:
    GeneInteraction()
    {
      g2 = new Gene;
    }
    ~GeneInteraction()
    {
      delete g2;
    }
   bool supresses_on()
   {
     return supresses;
   }
   bool enhances_on()
   {
     return enhances;
   }
};
#endif
