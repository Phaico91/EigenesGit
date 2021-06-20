#include <stdio.h>

int main()
{
  char *msg = "Hello there";
  char *nums = "1 3 5 7 9";
  const int len = 5;
  char s[len+1], t[len+1], buf[2*len + 2*len + 3 + 1];
  int a, b, c, n, m, k;

  n = sscanf(msg, "%s %s", s, t);
  m = sscanf(nums, "%d %d %d", &a, &b, &c);
  k = sprintf(buf,"|%*s|%-*s|", 2*len,t,2*len,s);
  printf("n=%d, m=%d, k=%d\n", n, m, k);
  printf("s=%s\n",buf);
  printf("a=%d, answer=%d\n", a, b+c);
  return 0;
}
