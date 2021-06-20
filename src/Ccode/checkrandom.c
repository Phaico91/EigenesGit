#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  const unsigned long trials = 100000000; /* 10^8 */
#define MAXNUM 6
  unsigned long idx, randtrial, randnum, randdist[MAXNUM+1] = {0};
  srand48(366292341L); /* use some large prime number */
  for (randtrial = 0; randtrial < trials; randtrial++) {
    randnum = 1 + (unsigned long) (drand48() * MAXNUM);
    randdist[randnum]++;
  }
  for (idx = 1; idx <= MAXNUM; idx++)
    printf("%lu %lu (%.2f)\n",idx,randdist[idx],
                            100.0 * (double) randdist[idx]/trials);
  return EXIT_SUCCESS;
}
