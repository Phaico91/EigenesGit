#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>

typedef unsigned short Bitsequence;

char *bitsequence_to_string(Bitsequence bs)
{
  unsigned int idx;
  const unsigned int bits = CHAR_BIT * sizeof (Bitsequence);
  Bitsequence mask;
  char *buffer;

  buffer = malloc((bits + 1) * sizeof *buffer);
  assert(buffer != NULL);
  for (idx = 0, mask = ((Bitsequence) 1) << (bits - 1);
       mask > 0; idx++, mask >>= 1)
  {
    buffer[idx] = (bs & mask) ? '1' : '0';
  }
  assert(idx == bits);
  buffer[idx] = '\0';
  return buffer;
}

int main(int argc,char *argv[])
{
  int idx;

  for (idx = 1; idx < argc; idx++)
  {
    char *buffer;
    long readlong;

    if (sscanf(argv[idx],"%ld",&readlong) != 1 || readlong < 0)
    {
      fprintf(stderr,"Usage: %s [<num_1> <num_2> ...  <num_n>]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
    buffer = bitsequence_to_string((Bitsequence) readlong);
    printf("%4ld %s\n",readlong,buffer);
    free(buffer);
  }
  return EXIT_SUCCESS;
}
