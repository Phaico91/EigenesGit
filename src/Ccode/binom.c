#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include "array2dim.h"

typedef unsigned long Ulong;

Ulong faculty(Ulong n)
{
  Ulong prod;

  for (prod = 1; n > 0; n--) prod *= n;
  return prod;
}

Ulong evalbinom1(Ulong n, Ulong k) {
  if (k > n) return 0;
  return faculty(n)/(faculty(k) * faculty(n-k));
}

Ulong safeadd(Ulong a,Ulong b)
{
  if (a == 0) return b;
  if (b == 0) return a;
  if (a > ULONG_MAX - b)
  {
    fprintf(stderr,"%s: overflow(%lu + %lu)\n",__func__,a,b);
    exit(EXIT_FAILURE);
  }
  return a + b;
}

Ulong safemult(Ulong a, Ulong b)
{
  if (a == 0 || b == 0)
    return 0;
  if (a > ULONG_MAX/b)
  {
    fprintf(stderr,"%s: overflow(%lu * %lu)\n",
            __func__,a,b);
    exit(EXIT_FAILURE);
  }
  return a * b;
}

Ulong safefaculty(Ulong n)
{
  Ulong prod;
  for (prod = 1; n > 0; n--)
  {
    prod = safemult(prod,n);
  }
  return prod;
}

Ulong safeevalbinom1(Ulong n, Ulong k)
{
  return safefaculty(n)/safemult(safefaculty(k),
                                 safefaculty(n-k));
}

Ulong evalbinom2(Ulong n, Ulong k)
{
  Ulong j, result;

  if (k == 0 || n == k)
    return 1;
  if (n == 0)
    return 0;
  for (result = 1, j=1; j<=k; j++)
    result = safemult(result,n - k + j)/j;
  return result;
}

Ulong evalbinom3(Ulong n, Ulong k)
{
  if (k == 0 || n == k)
    return 1;
  if (n == 0)
    return 0;
  return safeadd(evalbinom3(n-1,k-1),evalbinom3(n-1,k));
}

typedef struct
{
  bool defined;
  unsigned long value;
} DefinedValue;

Ulong evalbinom4memo(DefinedValue **tab, Ulong n, Ulong k)
{
  if (k == 0 || n == k) return 1;
  if (n == 0) return 0;
  if (!tab[n-1][k-1].defined)
  {
    tab[n-1][k-1].value = evalbinom4memo(tab,n-1,k-1);
    tab[n-1][k-1].defined = true;
  }
  if (!tab[n-1][k].defined)
  {
    tab[n-1][k].value = evalbinom4memo(tab,n-1,k);
    tab[n-1][k].defined = true;
  }
  return safeadd(tab[n-1][k-1].value,tab[n-1][k].value);
}

Ulong evalbinom4(Ulong n, Ulong k)
{
  Ulong ret, i, j;
  DefinedValue **tab;

  array2dim_malloc(tab,DefinedValue,n+1,k+1);
  for (i=0; i<=n; i++)
    for (j=0; j<=k; j++)
      tab[i][j].defined = false;
  ret = evalbinom4memo(tab,n,k);
  array2dim_delete(tab);
  return ret;
}

/*
Ulong evalbinom4(Ulong n, Ulong k)
{
  Ulong a[100][50], i, j, c;

  a[1][1]=1;
  a[2][1]=1;
  a[2][2]=2;
  a[2][3]=1;
  for (i=3; i<=n; i++)
  {
    a[i][1]=1;
    for (j=2,c=3; j<=i; c++, j++)
    {
      a[i][j]=a[i-1][c-1]+a[i-1][c];
    }
    a[i][j]=1;
  }
  return a[n][k];
}
*/

Ulong evalbinom5(Ulong n, Ulong k)
{
  if (k > n) return 0;
  return (Ulong) exp(lgamma(n+1) - lgamma(k+1)
                                 - lgamma(n-k+1));
}

int main(int argc,char *argv[])
{
  long readlong;
  Ulong n, k;

  if (argc != 3)
  {
    fprintf(stderr,"Usage: %s <n> <k>\n",argv[0]);
    return EXIT_FAILURE;
  }
  if (sscanf(argv[1],"%ld",&readlong) != 1 || readlong < 0)
  {
    fprintf(stderr,"%s: first argument must be non-negative integer\n",
                   argv[0]);
    return EXIT_FAILURE;
  }
  n = (Ulong) readlong;
  if (sscanf(argv[2],"%ld",&readlong) != 1 || readlong < 0)
  {
    fprintf(stderr,"%s: second argument must be non-negative integer\n",
                   argv[0]);
    return EXIT_FAILURE;
  }
  k = (Ulong) readlong;
  printf("2: %lu choose %lu = %lu\n",n,k,evalbinom2(n,k));
  printf("4: %lu choose %lu = %lu\n",n,k,evalbinom4(n,k));
  printf("5: %lu choose %lu = %lu\n",n,k,evalbinom5(n,k));
  printf("3: %lu choose %lu = %lu\n",n,k,evalbinom3(n,k));
  printf("1: %lu choose %lu = %lu\n",n,k,evalbinom1(n,k));
  /*printf("1': %lu choose %lu = %lu\n",n,k,safeevalbinom1(n,k));*/
  return EXIT_SUCCESS;
}
