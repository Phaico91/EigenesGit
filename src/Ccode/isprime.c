#include <stdbool.h>
#include <assert.h>

/* see also
   http://www.wikihow.com/Check-if-a-Number-Is-Prime
   http://stackoverflow.com/questions/1032427/efficient-storage-of-prime-numbers
*/

bool isprime (unsigned long n)
{
  unsigned long i, w;

  if (n == 2 || n == 3)
    return true;
  if (n % 2 == 0)
    return false;
  if (n % 3 == 0)
    return false;
  for (i = 5, w = 2; i * i <= n; i += w, w = 6 - w)
  {
    if (n % i == 0)
      return false;
  }
  return true;
}
