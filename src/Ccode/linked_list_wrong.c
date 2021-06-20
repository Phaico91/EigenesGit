#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct ListElem
{
  int data;
  struct ListElem *next;
};

typedef struct ListElem
               ListElem;

#ifdef SUPERFLUOUS
if (tail == NULL) /* empty list */
{
  head = tail = malloc(sizeof *tail);
} else /* non-empty list */
{
  tail->next = malloc(sizeof *tail);
  tail = tail->next;
}
tail->data = data;
tail->next = NULL;
#endif

void linked_list_append(ListElem *head,
                        ListElem *tail,
                        int data) {
  if (tail == NULL) /* empty list */
    head = tail = malloc(sizeof *tail);
  else /* non-empty list */
  {
    tail->next = malloc(sizeof *tail);
    tail = tail->next;
  }
  tail->data = data;
  tail->next = NULL;
}

void linked_list_show(const ListElem *head) {
  const ListElem *ptr;

  printf("[");
  for (ptr = head; ptr != NULL; ptr = ptr->next)
    printf("%s%d",ptr == head ? "" : ", ",
                  ptr->data);
  printf("]\n");
}

int main(void)
{
  ListElem *start = NULL, *end = NULL;
  int i;

  for (i = 0; i < 10; i++)
  {
    linked_list_append(start,end,i);
  }
  linked_list_show(start);
  return EXIT_SUCCESS;
}
