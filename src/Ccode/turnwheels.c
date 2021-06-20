#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "turnwheels.h"

#define MAXNUMOFWHEELS 18

struct Turningwheel
{
  const unsigned int *sizes;
  bool instartstate;
  unsigned int wheels[MAXNUMOFWHEELS],
               numofwheels,
               wheelid;
};

Turningwheel *turningwheel_new(unsigned int numofwheels,
                               const unsigned int *sizes)
{
  unsigned int i;
  Turningwheel *tw;

  assert(numofwheels > 0);
  assert(numofwheels < (unsigned int) MAXNUMOFWHEELS);
  tw = malloc(sizeof *tw);
  assert(tw != NULL);
  for (i=0; i<numofwheels; i++)
  {
    tw->wheels[i] = 0;
  }
  tw->numofwheels = numofwheels;
  tw->sizes = sizes;
  tw->wheelid = numofwheels - 1; /* innermost loop */
  tw->instartstate = true; /* for initial output */
  return tw;
}

void turningwheel_delete(Turningwheel *tw)
{
  if (tw != NULL)
  {
    free(tw);
  }
}

const unsigned int *turningwheel_next(Turningwheel *tw)
{
  if (tw->instartstate)
  {
    tw->instartstate = false;
    return tw->wheels; /* initial output */
  }
  for (;;)
  {
    tw->wheels[tw->wheelid]++; /* turn wheel[wheelid] */
    if (tw->wheels[tw->wheelid] == tw->sizes[tw->wheelid])
    { /* end of loop wheelid */
      tw->wheels[tw->wheelid] = 0; /* init for next iteration of this loop */
      if (tw->wheelid == 0) /* aleady at end of outermost loop: done */
        return NULL;
      tw->wheelid--; /* turn to surrounding look */
    } else
    {
      tw->wheelid = tw->numofwheels-1; /* return to inner loop */
      return tw->wheels; /* return current state */
    }
  }
}
