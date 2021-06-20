#include <stdio.h>

void convert(void)
{
  int x = 100000;
  short s;

  s = x;
  printf("%d %hd\n", x, s);
}

int main(void)
{
  int i = 1, j = 12;     /* i and j are initialized */
  float f1, f2 = 1.2;  /* f2 is initialized */

  f1 = i;            /* implicit: f1 <- 1.0 */
  f1 = f2 + j;       /* implicit: f1 <- 1.2 + 12.0 */

  f1 = f2 + (float) j; /* explicit: f1 <- 1.2 + 12.0 */
  i = (int) f2;        /* explicit: i <- 1, 0.2 lost */
  printf("%f %f", f1, f2);
  convert();
  return 0;
}
