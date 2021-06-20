#include <stdlib.h>
#include <stdbool.h>

bool array_compare(const int *a,unsigned long alen,
                   const int *b,unsigned long blen)
{
  unsigned long idx;

  if (alen != blen)
    return false;
  for (idx = 0; idx < alen; idx++)
  {
    if (a[idx] != b[idx])
      return false;
  }
  return true;
}

bool array_compare_ptr(const int *a,unsigned long alen,
                       const int *b,unsigned long blen)
{
  const int *aptr, *bptr;

  if (alen != blen)
    return false;
  for (aptr = a, bptr = b; aptr < a + alen; aptr++, bptr++)
  {
    if (*aptr != *bptr)
      return false;
  }
  return true;
}

size_t array_find(const short *a,size_t alen,short elem)
{
  size_t idx;

  for (idx = 0; idx < alen; idx++)
  {
    if (a[idx] == elem)
    {
      return idx;
    }
  }
  return alen; /* elem not found */
}

bool array_occurs(const short *a,size_t alen,short elem)
{
  return array_find(a,alen,elem) < alen ? true : false;
}

const short *array_find_ptr(const short *a,
                            size_t alen,
                            short elem) {
  const short *aptr;

  for (aptr = a; aptr < a + alen; aptr++)
    if (*aptr == elem)
      return aptr;
  return NULL; /* value of pointer not 
          referring to any memory cell */
}

void array_copy(int *dest,const int *src,unsigned long len)
{
  unsigned long idx;

  for (idx = 0; idx < len; idx++)
  {
    dest[idx] = src[idx];
  }
}
