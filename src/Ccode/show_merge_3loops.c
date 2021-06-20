#include <stdio.h>
#include <stdlib.h>
void show_sorted_3loops(
              const int *a,int alen,
              const int *b,int blen)
{
  unsigned long aidx = 0, bidx = 0;
  while (aidx < alen && bidx < blen)
  { /* loop 1 */
    printf("%d\n",
           a[aidx] < b[bidx] ? a[aidx++]
                             : b[bidx++]);
  }
  while (aidx < alen) /* loop 2 */
    printf("%d\n",a[aidx++]);
  while (bidx < blen) /* loop 3 */
    printf("%d\n",b[bidx++]);
}

void show_sorted_3loops_ptrs(
           const int *a,int alen,
           const int *b,int blen)
{
  const int *aptr = a, *bptr = b;
  while (aptr < a + alen &&
         bptr < b + blen)
  {
    printf("%d\n",
           *aptr < *bptr ? *aptr++
                         : *bptr++);
  }
  while (aptr < a + alen)
    printf("%d\n",*aptr++);
  while (bptr < b + blen)
    printf("%d\n",*bptr++);
}

int main(void)
{
  int a[] = {1,3,5,8,12},
      b[] = {0,7,7,15};
  show_sorted_3loops(a,sizeof a/sizeof a[0],
                     b,sizeof b/sizeof b[0]);
  show_sorted_3loops_ptrs(a,sizeof a/sizeof a[0],
                          b,sizeof b/sizeof b[0]);
  return EXIT_SUCCESS;
}
