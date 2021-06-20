#include <assert.h>
#include <string.h>

unsigned long binarysearch(const char * *table,
                           unsigned long entries,
                           const char *searchkey)
{
  unsigned long left = 0, right = entries - 1;

  assert(entries > 0);
  while (left <= right) {
    unsigned long mid = left + (right - left)/2;
    int cmp = strcmp(searchkey,table[mid]);
    if (cmp == 0) return mid; /* found */
    if (cmp < 0) {
      if (mid == 0) break; /* to prevent value < 0 for right */
      right = mid - 1; /* search in [left,mid-1] */
    } else
      left = mid + 1;  /* search in [mid+1,right] */
  }
  return entries; /* not found */
}

unsigned long binarysearch_ptr(const char * *table,
                               unsigned long entries,
                               const char *searchkey)
{
  const char * *left = table, * *right = table + entries - 1;

  while (left <= right) {
    const char * *mid = left + (right - left)/2;
    int cmp = strcmp(searchkey,*mid);
    if (cmp == 0)
      return (unsigned long) (mid - table); /* found */
    if (cmp < 0)
      right = mid - 1;
    else
      left = mid + 1;
  }
  return entries; /* not found */
}
