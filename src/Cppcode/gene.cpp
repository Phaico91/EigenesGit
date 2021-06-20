#include "gene.hpp"
void Gene::length_set(int l) {
  this->length = l;
}
int Gene::length_get(void) const
{
  return this->length; // this refers to current object
}
