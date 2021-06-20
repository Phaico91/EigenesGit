#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H
unsigned long binarysearch(const char **table,
                           unsigned long entries,
                           const char *searchkey);
unsigned long binarysearch_ptr(const char **table,
                               unsigned long entries,
                               const char *searchkey);
#endif
