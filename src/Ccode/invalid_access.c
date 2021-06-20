#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
  int i, finalidx, arr[9];

  if (argc != 2 || 
      sscanf(argv[1],"%d",
             &finalidx) != 1)
  {
    fprintf(stderr,
           "Usage: %s <index>\n",
            argv[0]);
    return EXIT_FAILURE;
  }
  for (i=0; i <= finalidx; i++)
    arr[i] = i;
  for (i=0; i <= finalidx; i++)
    printf("arr[%d]=%d\n",i,
           arr[i]);
  return EXIT_SUCCESS;
}
