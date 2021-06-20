#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

int main(void)
{
  LinkedList *ll = linked_list_new();
  int i;

  for (i = 0; i < 10; i++)
  {
    linked_list_append(ll, i);
  }
  printf("list of size %lu\n",
         linked_list_size(ll));
  linked_list_show(ll);
  linked_list_delete(ll);
  return EXIT_SUCCESS;
}
