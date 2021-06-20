#include <stdlib.h>
int function(void)
{
  int r;

  srand48(366292341L);
  r = 1 + (int) (drand48() * 6.0);
  return r;
}
