#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  int idx;

  for (idx = 0; idx <= UCHAR_MAX; idx++)
  {
    if (isspace(idx))
    {
      printf("%d>%c<\n",idx,(char) idx);
    }
  }
  return EXIT_SUCCESS;
}
