/* This is a slightly modified version of the
   program from figure 11.5 of the book of A. Moffat */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct
{
  FILE *fp;   /* reference to file */
  char *line; /* buffer for line in file referred to by fp */
  bool valid; /* can file still be processed? */
} Fileinfo;

static unsigned long find_smallest_line(const Fileinfo *finfo_tab,
                                        unsigned long numoffiles)
{
  unsigned long f, smallest = numoffiles; /* undefined */

  for (f = 0; f < numoffiles; f++) {
    if (finfo_tab[f].valid &&
        (smallest == numoffiles ||
         strcmp(finfo_tab[f].line,finfo_tab[smallest].line) < 0))
    {
      smallest = f;
    }
  }
  return smallest;
}

static void mergefiles(const char **filelist,int numoffiles,
                       unsigned long maxline)
{
  Fileinfo *finfo_tab = malloc(numoffiles * sizeof *finfo_tab);
  unsigned long f, open_files = 0;
  for (f = 0; f < numoffiles; f++) {
    finfo_tab[f].line = malloc((maxline+1) * sizeof (char));
    finfo_tab[f].fp = fopen(filelist[f],"r");
    if (finfo_tab[f].fp == NULL) {
      fprintf(stderr,"cannot open file %s\n",filelist[f]);
      exit(EXIT_FAILURE);
    }
    finfo_tab[f].valid = true;
  } /* next for-loop initializes open_files and f */
  for (open_files = numoffiles, f = 0; f < numoffiles; f++) {
    if (fgets(finfo_tab[f].line,maxline,finfo_tab[f].fp) == NULL) {
      finfo_tab[f].valid = false;
      fclose(finfo_tab[f].fp);
      assert(open_files > 0);
      open_files--;
    }
  }
  while (open_files > 0) {
    unsigned long smallest = find_smallest_line(finfo_tab,numoffiles);
    assert(smallest < numoffiles);
    printf("%s",finfo_tab[smallest].line);
    if (fgets(finfo_tab[smallest].line,maxline,
              finfo_tab[smallest].fp) == NULL) {
      finfo_tab[smallest].valid = false;
      fclose(finfo_tab[smallest].fp);
      assert(open_files > 0);
      open_files--;
    }
  }
  for (f = 0; f < numoffiles; f++)
    free(finfo_tab[f].line);
  free(finfo_tab);
}

int main(int argc,const char **argv)
{
  if (argc <= 2)
  {
    fprintf(stderr,
            "Usage: %s <file_1> <file_2> [...file_n]\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }
  mergefiles(argv + 1,argc - 1,1024);
  exit(EXIT_SUCCESS);
}
