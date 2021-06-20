#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

static void distribution_update(unsigned long *dist,
                                const char *progname,
                                const char *filename)
{ /* update distribution for all characters in given file */
  int cc;
  FILE *fp = fopen(filename,"rb");
  if (fp == NULL)
  {
    fprintf(stderr,"%s: cannot open %s\n",progname,filename);
    exit(EXIT_FAILURE);
  }
  while ((cc = fgetc(fp)) != EOF)
    dist[cc]++;
  (void) fclose(fp);
}

static void distribution_show(const unsigned long *dist)
{
  int i;

  for (i = 0; i <= UCHAR_MAX; i++)
  {
    if (dist[i] > 0)
    {
      if (iscntrl(i)) /* is it a control character? */
        printf("\\%d: %lu\n",i,dist[i]);
      else
        printf("%c: %lu\n",(char) i,dist[i]);
    }
  }
}

int main(int argc, char *argv[])
{
  int i;
  unsigned long dist[UCHAR_MAX+1] = {0};

  if (argc < 2)
  {
    fprintf(stderr,
      "Usage: %s <infile1>...<infilen>\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }
  for (i = 1; i < argc; i++)
  {
    distribution_update(dist,argv[0],
                        argv[i]);
  }
  distribution_show(dist);
  exit(EXIT_SUCCESS);
}
