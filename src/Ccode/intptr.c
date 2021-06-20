#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void loop_with_print(void)
{
int iteration = 0, a = 0, *intptr = &a, b = 0;

while (*intptr < 5) /* dereference. intptr */
{
  if (intptr == &a)   /* compare addresses */
    intptr = &b, printf("%d: intptr=b\n",iteration);
  else
    intptr = &a, printf("%d: intptr=a\n",iteration);
  (*intptr)++;        /* increment the contents of the memory
                         cell intptr refers to */
  assert((intptr == &b && iteration % 2 == 0 && iteration <= 8) ||
         (intptr == &a && iteration % 2 == 1 && iteration <= 7));
  iteration++; 
}
printf("iteration=%d\n",iteration);
}

void loop_silent(void)
{
int iteration = 0, a = 0, *intptr = &a, b = 0;

while (*intptr < 5) /* dereference. intptr */
{
  intptr = (intptr == &a) ? &b : &a; /* switch between addresses */
  (*intptr)++;  /* increment the contents of the 
                   memory cell intptr refers to: either b or a */
  assert((intptr == &b && iteration % 2 == 0 && iteration <= 8) ||
         (intptr == &a && iteration % 2 == 1 && iteration <= 7));
  iteration++; 
}
printf("iteration=%d\n",iteration);
}

int main()
{
  loop_with_print();
  exit (EXIT_SUCCESS);
}
