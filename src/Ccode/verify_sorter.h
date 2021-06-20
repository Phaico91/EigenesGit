#ifndef VERIFY_SORTER_H
#define VERIFY_SORTER_H
void verify_sorter(void (*sorter)(unsigned long *a,
                                  unsigned long len),
                   unsigned long runs,
                   unsigned long numofvalues);
#endif
