#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void show_sorted(const int *a,int alen,
                 const int *b,int blen)
{ 
  unsigned long aidx = 0, bidx = 0;
  while (1) {
    if (aidx < alen) /* a not exhausted */
    {
      if (bidx >= blen || /* b exhausted */
          a[aidx] < b[bidx])
        printf("%d\n",a[aidx++]);/* 1 */
      else
        printf("%d\n",b[bidx++]);/* 2 */
    } else
    {
      if (bidx < blen) /* b not exhausted */
        printf("%d\n",b[bidx++]);/* 3 */
      else /* a and b exhausted */
        return;                  /* 4 */
    }
  }
}

#ifdef IGNORE_THIS
while (1) {
  if (aidx < alen) /* a not exhausted */
  {
    if (bidx >= blen || /* b exhausted */
        a[aidx] < b[bidx])
      printf("%d\n",a[aidx++]);/* 1 */
    else
      printf("%d\n",b[bidx++]);/* 2 */
  } else
  {
    if (bidx < blen) /* b not exhausted */
      printf("%d\n",b[bidx++]);/* 3 */
    else
      return;                  /* 4 */
  }
}
#endif

int main(void)
{
  int a[] = {1,3,5,8,12},
      b[] = {0,7,7,15};
  show_sorted(a,sizeof a/sizeof a[0],
              b,sizeof b/sizeof b[0]);
  return EXIT_SUCCESS;
}
