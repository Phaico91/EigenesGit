#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "linked_list.h"

struct ListElem
{
  int data;
  struct ListElem *next;
};

typedef struct ListElem
               ListElem;

struct LinkedList
{
  ListElem *head, *tail;
  size_t num_elems;
};

LinkedList *linked_list_new(void)
{
  LinkedList *ll = malloc(sizeof *ll);
  ll->head = ll->tail = NULL;
  ll->num_elems = 0;
  return ll;
}

void linked_list_append(LinkedList *ll, int data)
{
  assert (ll != NULL);
  if (ll->tail == NULL)
  {
    ll->head = ll->tail = malloc(sizeof *ll->tail);
  } else
  {
    ll->tail->next = malloc(sizeof *ll->tail);
    ll->tail = ll->tail->next;
  }
  ll->tail->data = data;
  ll->tail->next = NULL;
  ll->num_elems++;
}

size_t linked_list_size(const LinkedList *ll)
{
  assert(ll != NULL);
  return ll->num_elems;
}

void linked_list_show(const LinkedList *ll)
{
  const ListElem *ptr;

  printf("[");
  for (ptr = ll->head; ptr != NULL; ptr = ptr->next)
  {
    printf("%s%d",ptr == ll->head ? "" : ", ", ptr->data);
  }
  printf("]\n");
}

void linked_list_delete(
           LinkedList *ll)
{
  ListElem *ptr;

  assert (ll != NULL);
  ptr = ll->head;
  while (ptr != NULL)
  {
    ListElem *tmp = ptr;

    ptr = ptr->next;
    free(tmp);
  }
  free(ll);
}
