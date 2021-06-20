#include <stdio.h>
#include <math.h>

double difference(int k) {
  return pow(10.0,k) + 0.3 - (pow(10.0,k) + 0.1 + 0.2);
}

int main(void)
{
  {
  int a = 1, b = 2, expected = 3;
  printf("%s\n",a + b == expected ? "True" : "False");
  }
  {
  double a = 0.1, b = 0.2, expected = 0.3;
  printf("%s\n",a + b == expected ? "True" : "False");
  }
  printf("%.17f\n%.17f\n%.17f\n%.17f\n",0.1,0.2,0.1 + 0.2,0.3);
  {
  double a = 0.1, b = 0.2, expected = 0.3,
         computed = a + b,
         diff = fabs(expected - computed),
         tol = 1e-15;
  printf("%s\n",diff < tol ? "True" : "False");
  }
  {
int k;
  /*lst{Roundingabsolutedifference}*/
for (k=1; k <= 10; k++)
  printf("%2d\t%+.17f\t%.17f\n",k,difference(k),pow(10.0,k-16));
  /*lstend*/
  /*lst{Roundingrelativedifference}*/
for (k=1; k <= 10; k++) {
  double d = difference(k),
         v = pow(10.0,k) + 0.3;
  printf("%2d\t%.2g\n",k,d/v);
}
  /*lstend*/
  }
  return 0;
}
