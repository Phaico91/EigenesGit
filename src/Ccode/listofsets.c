#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listofsets.h"

struct Listofsets
{
  long *inputtab;
  unsigned long setnextfree, setallocated,
                elemnextfree, elemallocated, *endidx;
};

Listofsets *listofsets_new(void)
{
  Listofsets *listofsets;

  listofsets = malloc(sizeof *listofsets);
  assert(listofsets != NULL);
  listofsets->endidx = NULL;
  listofsets->inputtab = NULL;
  listofsets->elemnextfree = 0;
  listofsets->elemallocated = 0;
  listofsets->setnextfree = 0;
  listofsets->setallocated = 0;
  return listofsets;
}

void listofsets_delete(Listofsets *listofsets)
{
  if (listofsets != NULL)
  {
    free(listofsets->endidx);
    free(listofsets->inputtab);
    free(listofsets);
  }
}

void listofsets_newset(Listofsets *listofsets)
{
  if (listofsets->setnextfree >= listofsets->setallocated)
  {
    const size_t addsize = 10;
    listofsets->endidx
      = realloc(listofsets->endidx,(listofsets->setallocated + addsize) *
                                   sizeof *listofsets->endidx);
    assert(listofsets->endidx != NULL);
    listofsets->setallocated += addsize;
  }
  if (listofsets->setnextfree > 0)
  {
    listofsets->endidx[listofsets->setnextfree]
      = listofsets->endidx[listofsets->setnextfree-1];
  } else
  {
    listofsets->endidx[0] = 0;
  }
  listofsets->setnextfree++;
}

void listofsets_add(Listofsets *listofsets,long value)
{
  if (listofsets->elemnextfree >= listofsets->elemallocated)
  {
    const size_t addsize = 20;
    listofsets->inputtab
      = realloc(listofsets->inputtab,(listofsets->elemallocated + addsize) *
                                     sizeof *listofsets->inputtab);
    assert(listofsets->inputtab != NULL);
    listofsets->elemallocated += addsize;
  }
  listofsets->inputtab[listofsets->elemnextfree++] = value;
  listofsets->endidx[listofsets->setnextfree-1]++;
}

unsigned int *listofsets_sizes(Listofsets *listofsets)
{
  unsigned long idx;
  unsigned int *sizes;

  sizes = malloc(listofsets->setnextfree * sizeof *sizes);
  assert(sizes != NULL);
  for (idx=0; idx<listofsets->setnextfree; idx++)
  {
    unsigned long firstindex = idx == 0 ? 0 : listofsets->endidx[idx-1];
    sizes[idx] = (unsigned int) (listofsets->endidx[idx] - firstindex);
  }
  return sizes;
}

long listofsets_getelem(const Listofsets *listofsets,unsigned long setnum,
                        unsigned int idx)
{
  unsigned long firstindex = setnum == 0 ? 0 : listofsets->endidx[setnum-1];
  return listofsets->inputtab[firstindex+idx];
}

unsigned long listofsets_numofsets(const Listofsets *listofsets)
{
  return listofsets->setnextfree;
}

void listofsets_show(const Listofsets *listofsets)
{
  unsigned long idx;
  for (idx=0; idx<listofsets->setnextfree; idx++)
  {
    unsigned long k, firstindex = idx == 0 ? 0 : listofsets->endidx[idx-1];
    printf("set %lu from index %lu to %lu with",
            idx,firstindex,listofsets->endidx[idx]-1);
    for (k=firstindex; k<listofsets->endidx[idx]; k++)
    {
      printf(" %lu",listofsets->inputtab[k]);
    }
    printf("\n");
  }
}
