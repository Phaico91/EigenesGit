#include <stdio.h>

void mycaller(void (*f)(int), int param) {
  f(param); /* call function f with param */
}

void myproc (int d) {
  printf("%d\n", d); /* ... do something with d */
}

int main(void) {
  myproc(10);           /* call myproc with parameter 10*/
  mycaller(myproc, 10); /* and do the same again */
  return 0;
}
