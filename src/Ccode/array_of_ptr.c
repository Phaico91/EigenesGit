#include <stdio.h>

int main(void)
{
  char *ptr1 = "LaTeX";
  char *ptr2 = "MS-Word";
  char *ptr3 = "OpenOffice";
  char *arr[3]; /* an array of 3 char pointers */

  arr[0] = ptr1;
  arr[1] = ptr2;
  arr[2] = ptr3;
  printf("%s\n%s\n%s\n", arr[0], arr[1], arr[2]);
  return 0;
}
