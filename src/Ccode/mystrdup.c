#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

char *mystrdup(const char *s)  
{ /* equivalent to strdup */
  char *u = malloc(strlen(s) + 1);
  assert(u != NULL);
  strcpy(u, s);
  return u;
}

void showcapitalized(const char *s) {
  char *copy = mystrdup(s);
  copy[0] = toupper(copy[0]);
  printf("%s\n",copy); /* copy leaks */
}

int main(void) {
  showcapitalized("memory");
  return EXIT_SUCCESS;
}
