#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "verify_sorter.h"

static void mergestep(
    unsigned long *buf, /* refers to array of length >= alen+blen */
    const unsigned long *a,size_t alen,
    const unsigned long *b,size_t blen)
{
  const unsigned long *aptr = a, *bptr = b; /* read from aptr, bptr */
  unsigned long *ptr = buf; /* write to memory pointed to by ptr */

  while (aptr < a + alen && bptr < b + blen)
  { /* there are elements left in both arrays */
    if (*aptr < *bptr)
      *ptr++ = *aptr++; /* take elem from a */
    else
      *ptr++ = *bptr++; /* take elem from b */
  }
  if (aptr < a + alen) /* move remaining from a to buf */
    memcpy(ptr,aptr,(a + alen - aptr) * sizeof *a);
  if (bptr < b + blen) /* move remaining from b to buf */
    memcpy(ptr,bptr,(b + blen - bptr) * sizeof *b);
}

void mergesort_r(unsigned long *a,unsigned long *buf,size_t len)
{
  if (len >= 2)
  {
    mergesort_r(a,      buf,      len/2);       /* sort 1st halve */
    mergesort_r(a+len/2,buf+len/2,len - len/2); /* sort 2dn halve */
    mergestep(buf,a,len/2,a+len/2,len - len/2);
    memcpy(a,buf,len * sizeof *a); /* copy merged element back */
  }
}

void mergesort_all(unsigned long *a,size_t len)
{
  unsigned long *buf = malloc(len * sizeof *a);

  mergesort_r(a,buf,len);
  free(buf);
}

#ifdef DEBUG
static void check_order(__attribute__ ((unused)) int line,unsigned long *arr,
                        size_t len)
{
  /*printf("line %d: ",line);
  for (size_t idx = 0; idx < len; idx++)
  {
    printf("%lu%c",arr[idx],idx == len - 1 ? '\n' : ' ');
  }*/
  for (size_t idx = 1; idx < len; idx++)
  {
    assert(arr[idx-1] <= arr[idx]);
  }
}
#else
#define check_order(A,B,C) /* Nothing */
#endif

static void mergestep_inplace(
    unsigned long *a,size_t alen, /* but has length alen + blen */
    unsigned long *b,size_t blen) /* starts at a + alen */
{
  assert(alen > 0 && blen > 0);
  assert(a + alen == b);
  unsigned long *aptr = a + alen - 1;
  unsigned long *bptr = b + blen - 1;

  check_order(__LINE__,a,alen);
  check_order(__LINE__,b,blen);

  while (bptr >= b)
  {
    if (*bptr < *aptr)
    {
      unsigned long *ptr;
      unsigned long tmp = *bptr;
      *bptr = *aptr;
      for (ptr = aptr; ptr > a && tmp < *(ptr-1); ptr--)
      {
        *ptr = *(ptr-1);
      }
      *ptr = tmp;
    }
    bptr--;
    check_order(__LINE__,a,alen);
    check_order(__LINE__,b,(unsigned long) (bptr - b + 1));
  }
  check_order(__LINE__,a,alen+blen);
}

void mergesort_inplace(unsigned long *a,size_t len)
{
  if (len >= 2)
  {
    mergesort_inplace(a,len/2);       /* sort 1st halve */
    mergesort_inplace(a+len/2,len - len/2); /* sort 2dn halve */
    mergestep_inplace(a,len/2,a+len/2,len - len/2);
  }
}

int main(int argc,char *argv[])
{
  long runs, numofvalues_read;

  if (argc != 3 || sscanf(argv[1],"%ld",&runs) != 1 || runs < 1L ||
      sscanf(argv[2],"%ld",&numofvalues_read) != 1 || numofvalues_read < 1L)
  {
    fprintf(stderr,"Usage: %s <runs> <numofvalues>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  verify_sorter(mergesort_all,(unsigned long) runs,
                (unsigned long) numofvalues_read);
  /*verify_sorter(mergesort_inplace,(unsigned long) runs,
                (unsigned long) numofvalues_read);*/
  return EXIT_SUCCESS;
}
