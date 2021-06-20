#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "binarysearch.h"

static unsigned long linearsearch(const char * *table,
                                  unsigned long entries,
                                  const char *key)
{
  unsigned long idx;
  for (idx = 0; idx < entries; idx++)
  {
    int cmp = strcmp(key,table[idx]); /* compare strings */
    if (cmp == 0)
      return idx; /* found: key == table[idx], idx < entries */
    if (cmp < 0)
      break; /* key < table[idx]<=table[idx+1]... */
  }
  return entries;  /* not found */
}

static void verifysearch(unsigned long (*search)(const char * *,
                                                 unsigned long,
                                                 const char *),
                         const char * *tab,unsigned long elems)
{
  unsigned long idx;

  for (idx = 0; idx < elems; idx++)
  {
    unsigned long nameindex = search(tab,elems,tab[idx]);
    assert(nameindex == idx);
  }
  printf("verification done\n");
}

int main(int argc,char *argv[])
{
  int i;
  unsigned long entries, nameindex;
  const char *names[] = {"Alf","Bea",
    "Ines","Kim","Mike","Nash","Pete",
    "Ro","Ruth","Sara","Su","Susi",
    "Tim","Tom","Xi"};

  printf("sizeof names=%ld\n",sizeof names);
  printf("sizeof names[0]=%ld\n",
          sizeof names[0]);
  entries = sizeof names/sizeof names[0];
  printf("entries=%lu\n",entries);
  for (i = 1; i < argc; i++)
  {
    nameindex = linearsearch(names,entries,
                             argv[i]);
    if (nameindex < entries)
      printf("%s found at pos %lu\n",argv[i],
                                  nameindex);
    else
      printf("%s not found\n",argv[i]);
  }
  verifysearch(linearsearch,names,entries);
  verifysearch(binarysearch_ptr,names,entries);
  verifysearch(binarysearch,names,entries);
  return EXIT_SUCCESS;
}
