#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "turnwheels.h"
#include "listofsets.h"

int main(int argc,char *argv[])
{
  char *ptr;
  int idx;
  long readlong;
  Listofsets *listofsets;
  Turningwheel *tw;
  unsigned long setnum, numofsets;
  unsigned int *sizes;
  const unsigned int *wheels;

  listofsets = listofsets_new();
  /* reading the sets from argv */
  for (idx=1; idx<argc; idx++)
  {
    listofsets_newset(listofsets);
    ptr = argv[idx];
    while (sscanf(ptr,"%ld",&readlong) == 1)
    {
      listofsets_add(listofsets,readlong);
      ptr = strchr(ptr,(int) ' ');
      if (ptr == NULL)
      {
        break;
      }
      ptr++;
    }
  }
  listofsets_show(listofsets);
  sizes = listofsets_sizes(listofsets);
  numofsets = listofsets_numofsets(listofsets);
  tw = turningwheel_new(numofsets,sizes);
  while ((wheels = turningwheel_next(tw)) != NULL)
  {
    for (setnum = 0; setnum<numofsets; setnum++)
    {
      printf(" %lu",listofsets_getelem(listofsets,setnum,
                                       wheels[setnum]));
    }
    printf("\n");
  }
  turningwheel_delete(tw);
  free(sizes);
  listofsets_delete(listofsets);
  return EXIT_SUCCESS;
}
