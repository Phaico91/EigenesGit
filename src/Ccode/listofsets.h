#ifndef LISTOFSETS_H
#define LISTOFSETS_H
typedef struct Listofsets Listofsets;

Listofsets *listofsets_new(void);
void listofsets_delete(Listofsets *listofsets);
void listofsets_newset(Listofsets *listofsets);

void listofsets_add(Listofsets *listofsets,long value);
unsigned int *listofsets_sizes(Listofsets *listofsets);

unsigned long listofsets_numofsets(const Listofsets *listofsets);

long listofsets_getelem(const Listofsets *listofsets,
                        unsigned long setnum,
                        unsigned int idx);

void listofsets_show(const Listofsets *listofsets);

#endif
