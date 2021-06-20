#ifndef GENE_H
#define GENE_H
#include <string>

class Gene {
  private:
    std::string name;
    int length;
  public:
    Gene(std::string n) { // inline constructor
      name =  n;
      length = 0;
    }
    void length_set(int l); // defined later
    std::string name_get(void) const
    {
      return name; 
    }
    int length_get(void) const;
    Gene(void) {
      name =  "";
      length = 0;
    }
    Gene(std::string n,int l) {
      name =  n;
      length = l;
    }
    Gene(const Gene &g) {
      name = g.name;
      length = 0;
    }
};
#endif
