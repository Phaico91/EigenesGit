#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
typedef unsigned long Ulong;
typedef struct Set Set; /* only know the name of the type */
Ulong set_size(const Set *); /* deliver size of set */
Ulong set_get(const Set *set,Ulong i); /* ith element */

void cartproduct2(const Set *s0,const Set *s1)
{
  Ulong i0, i1;

  for (i0=0; i0<set_size(s0); i0++)
    for (i1=0; i1<set_size(s1); i1++)
      printf("%lu %lu\n",set_get(s0,i0),set_get(s1,i1));
}

void cartproduct3(const Set *s0,const Set *s1,const Set *s2)
{
  Ulong i0, i1, i2;

  for (i0=0; i0<set_size(s0); i0++)
    for (i1=0; i1<set_size(s1); i1++)
      for (i2=0; i2<set_size(s2); i2++)
        printf("%lu %lu %lu\n",set_get(s0,i0),set_get(s1,i1),
                               set_get(s2,i2));
}

void cartproduct2singleloop(const Set **sets) /* 2 sets */
{
  Ulong w[2] = {0}, loopid = 1; /* 0=outer, 1=inner */

  printf("%lu %lu\n",set_get(sets[0],0),set_get(sets[1],0));
  for (;;)
  {
    w[loopid]++; /* turn wheel loopid */
    if (w[loopid] == set_size(sets[loopid])) /* end of loop */
    {
      w[loopid] = 0; /* init for next iteration of this loop */
      if (loopid == 0) /* at end of outer loop: done */
        break;
      loopid--; /* now turn to outer loop */
    } else
    {
      printf("%lu %lu\n",set_get(sets[0],w[0]),
                         set_get(sets[0],w[1]));
      if (loopid < 1) loopid = 1; /* return to inner loop */
    }
  }
}

void cartproduct3singleloop(const Set **sets)
{
  Ulong w[3] = {0},
        loopid = 2; /* 0=outer, 1=middle, 2=inner */

  /* output */
  for (;;)
  {
    w[loopid]++; /* turn wheel loopid */
    if (w[loopid] == set_size(sets[loopid])) /* end of loop */
    {
      w[loopid] = 0; /* init for next iteration of this loop */
      if (loopid == 0) /* already at end of outermost loop: done */
        break;
      loopid--; /* now turn to surrounding loop */
    } else
    {
      /* output */
      if (loopid < 2) loopid = 2; /* return to inner loop */
    }
  }
}

void cardproduct_show(const Set **sets,Ulong k,const Ulong *w);

void cartproductsingleloop(const Set **sets,Ulong k)
{
  Ulong *w = calloc(k,sizeof *w),
        loopid = k-1; /* 0=outer, ..., k-1 = inner*/
  cardproduct_show(sets,k,w);
  for (;;)
  {
    w[loopid]++; /* turn wheel loopid */
    if (w[loopid] == set_size(sets[loopid])) /* end of loop */
    {
      w[loopid] = 0; /* init for next iteration of this loop */
      if (loopid == 0) /* already at end of outermost loop: done */
        break;
      loopid--; /* now turn to surrounding loop */
    } else
    {
      cardproduct_show(sets,k,w);
      if (loopid < k-1) loopid = k-1; /* return to inner loop */
    }
  }
  free(w);
}

struct Set {
  Ulong *elements, /* store the elements here */
        size, /* number of elements store */
        allocated; /* number of entries allocated */
};

Set *set_new(void) /* constructor for new empty set */
{
  Set *s = malloc(sizeof *s);
  assert(s != NULL);
  s->elements = NULL;
  s->size = s->allocated = 0;
  return s;
}

void set_delete(Set *set) { /* destructor */
  if (set != NULL)
  {
    free(set->elements);
    free(set);
  }
}

bool set_member(const Set *set,Ulong elem) /* elem \in set? */
{
  Ulong idx;
  for (idx = 0; idx < set->size; idx++)
    if (set->elements[idx] == elem)
      return true;
  return false;
}

void set_add(Set *set,Ulong elem) /* add elem to set */
{
  if (!set_member(set,elem))
  {
    if (set->size == set->allocated)
    {
      set->allocated = set->allocated * 1.2 + 32;
      set->elements = realloc(set->elements,set->allocated);
    }
    set->elements[set->size++] = elem;
  }
}

Ulong set_size(const Set *set) /* size of set */
{
  return set->size;
}

Ulong set_get(const Set *set,Ulong i) /* get ith element from set */
{
  assert(i < set->size);
  return set->elements[i];
}

void cardproduct_show(const Set **sets,Ulong k,const Ulong *w)
{ /* show cartesian product for given wheel state w */
  Ulong idx;

  for (idx=0; idx<k; idx++)
  {
    printf("%lu%s",set_get(sets[idx],w[idx]),
                   idx < k-1 ? " " : "\n");
  }
}

Set **sets_read(const char **argv,Ulong k)
{ /* parse sets of strings from argv: "1 2" "5 7" are valid args */
  unsigned long idx; long readlong;
  const char *ptr;
  Set **sets = malloc(k * sizeof *sets);

  assert(sets != NULL);
  for (idx=0; idx<k; idx++)
  {
    sets[idx] = set_new();
    for (ptr = argv[idx+1];
         sscanf(ptr,"%ld",&readlong) == 1; ptr++)
    {
      set_add(sets[idx],(Ulong) readlong);
      ptr = strchr(ptr,(int) ' ');
      if (ptr == NULL)
        break;
    }
  }
  return sets;
}

void sets_delete(Set **sets,Ulong k) /* delete set of sets */
{
  unsigned long idx;

  for (idx=0; idx<k; idx++)
    set_delete(sets[idx]);
  free(sets);
}

int main(int argc,const char **argv)
{
  if (argc >= 2)
  {
    Ulong k = (Ulong) argc-1;
    Set **sets = sets_read(argv,k);

    cartproductsingleloop((const Set **) sets,k);
    sets_delete(sets,k);
  }
  return EXIT_SUCCESS;
}
