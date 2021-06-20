#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#define MAXWORD 100

int main(void)
{
  Tree *root = NULL; /* empty tree with 0 nodes */
  char *ptr, word[MAXWORD+1];

  while (fgets(word,MAXWORD,stdin) != NULL) /* read line */
  { /* and store it in word */
    if ((ptr = strrchr(word,'\n')) != NULL)
    {
      *ptr = '\0'; /* replace trailing \n by \0 */
    }
    root = tree_add(root,word);
  }
  tree_print(0,root);
  tree_print_tikz(0,root);

  tree_delete(root);
  return EXIT_SUCCESS;
}
