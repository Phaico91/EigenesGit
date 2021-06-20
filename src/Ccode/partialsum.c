#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#ifdef INCLUDE_THIS_CODE
  unsigned long k;
  for (k = 1; k < n; k++)
    a[k] += a[k - 1];
#endif

static void partialsums(int *a,
                        unsigned long n)
{
  unsigned long k;
  for (k = 1; k < n; k++)
    a[k] += a[k - 1];
}

static void partialsums_ptr(int *a,
                            unsigned long n)
{
  int *ap;
  for (ap = a+1; ap < a+n; ap++)
    *ap += *(ap-1);
}

int main(int argc, char **argv)
{
  int *a, *b,
      i;

  if (argc < 3) {
    fprintf(stderr, "give a list of integers as arguments (min 2)!\n");
    exit(EXIT_FAILURE);
  }
  a = malloc((size_t) (argc - 1) * sizeof *a);
  assert(a != NULL);
  b = malloc((size_t) (argc - 1) * sizeof *b);
  assert(b != NULL);

  for (i = 1; i < argc; i++)
  {
    if (sscanf(argv[i], "%d", a + (i - 1)) != 1)
    {
      fprintf(stderr,"%s: illegal input %s\n",argv[0],argv[i]);
      exit(EXIT_FAILURE);
    }
    b[i-1] = a[i-1];
  }
  partialsums(a, (unsigned long) (argc - 1));
  partialsums_ptr(b, (unsigned long) (argc - 1));
  for (i = 0; i < argc - 1; i++) {
    assert(a[i] == b[i]);
    printf("%d: %d\n",i ,a[i]);
  }
  free(a);
  free(b);
  return EXIT_SUCCESS;
}
