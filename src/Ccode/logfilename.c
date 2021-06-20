#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

char *logfilename(const char *directory,const char *prefix) {
  const char *suffix = "log";
  const int digits = 19;
  const size_t buffer_len = strlen(directory)
                            + 1 /* / */
                            + strlen(prefix)
                            + 1 /* _ */
                            + digits
                            + 1 /* . */
                            + strlen(suffix);
  char *buffer = malloc(buffer_len+1);
  int n = sprintf(buffer,"%s/%s_%0*ld.%s",directory,prefix,
                         digits,time(NULL),suffix);
  assert(n<=buffer_len);
  return buffer;
}

int main(void)
{
  char *filename = logfilename("/tmp","pfn2");
  printf("logfilename=%s\n",filename);
  free(filename);
  return EXIT_SUCCESS;
}
