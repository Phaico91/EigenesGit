#ifndef LINKED_LIST_H
#define LINKED_LIST_H
typedef struct LinkedList LinkedList;
LinkedList *linked_list_new(void);
void linked_list_append(LinkedList *ll,
                        int data);
size_t linked_list_size(
                const LinkedList *ll);
void linked_list_show(
                const LinkedList *ll);
void linked_list_delete(
                LinkedList *ll);
#endif
