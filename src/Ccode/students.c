#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int nstudents;
  printf("How many students does Columbia have ?:");
  /* Read input from terminal */
  if (scanf ("%d", &nstudents) != 1)
  {
    fprintf(stderr, "error reading input\n");
    exit(1);
  }
  printf("Columbia has %d students.\n", nstudents);
  return 0;
}
