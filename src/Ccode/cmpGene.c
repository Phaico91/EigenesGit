#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { char *name; int length; } Gene;

int gene_cmp(const void *aptr, const void *bptr) {
  const Gene *pa = (const Gene *) aptr;
  const Gene *pb = (const Gene *) bptr;

  return strcmp(pa->name,pb->name);
}

int main(void)
{
  Gene g1, g2;
  g1.name = "gene1";
  g1.length = 300;
  g2.name = "gene2";
  g2.length = 200;
  printf("gene_cmp(g1,g2)=%d\n",gene_cmp((const void *) &g1,
                                         (const void *) &g2));
  return EXIT_SUCCESS;
}
