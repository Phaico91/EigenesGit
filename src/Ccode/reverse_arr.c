#include <assert.h>
void reverse_array_ptr(int *arr,
                       unsigned long len)
{
  int *fwd, *bck;

  for (fwd = arr, bck = arr+len-1;
       fwd < bck; fwd++, bck--)
  {
    int tmp = *fwd;
    *fwd = *bck;
    *bck = tmp;
  }
}

void reverse_array_ptr2(int *arr,
                        unsigned long len)
{
  int *fwd, *bck;

  for (fwd = arr, bck = arr+len-1; fwd < bck; fwd++, bck--)
  {
    int tmp = *fwd;
    *fwd = *bck;
    *bck = tmp;
  }
}

void reverse_array_idx(int *arr,unsigned long len)
{
  unsigned long fwd, bck;

  assert(len > 0);
  for (fwd = 0, bck = len - 1; fwd < bck; fwd++, bck--)
  {
    int tmp = arr[fwd];
    arr[fwd] = arr[bck];
    arr[bck] = tmp;
  }
}
