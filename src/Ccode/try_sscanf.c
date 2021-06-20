#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
  unsigned int num;
  if (argc != 2)
  {
    fprintf(stderr,
            "Usage: %s <num>\n",
            argv[0]);
    return EXIT_FAILURE;
  }
  if (sscanf(argv[1],
             "%u",&num) != 1)
  {
    fprintf(stderr,
            "%s: cannot extract"
            " num from %s\n",
            argv[0],argv[1]);
    return EXIT_FAILURE;
  }
  printf("num=%u\n",num);
  return EXIT_SUCCESS;
}
