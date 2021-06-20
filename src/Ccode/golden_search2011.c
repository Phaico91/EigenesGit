#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <float.h>
#include "double_compare.h"

typedef struct
{
  double x1, x2, x3;
} Tripletofdoubles;

#define GOLD  ((sqrt(5.0) - 1.0)/2.0) /* 0.6180339887 */
#define VARPHI_CONSTANT GOLD

static double det_offset3(double dist)  /* x3 - x2 */
{
  return (1.0 - GOLD) * dist;
}

#define TOL sqrt(1.110223e-16)   /* argument is accuracy */
bool sufficiently_small(double l,double r)
{
  /* fabs(r - l) is the absulute distance between l and r */
  return fabs(r - l) < TOL ? true : false;
}

static double gsearch(double (*f)(double),
                      double x1,double x2,double x3)
{
  unsigned long iter;
  for (iter = 0; !sufficiently_small(x1,x3); iter++)
  {
    double x4;
    printf("%.8f %.8f %.8f\n",x1,x2,x3);
    if (double_compare(x1,x2) <= 0 && double_compare(x2,x3) <= 0)
    {
      assert(double_compare(x2 - x1,x3 - x2) < 0);
    } else
    {
      if (double_compare(x1,x2) >= 0 && double_compare(x2,x3) >= 0)
      {
        assert(double_compare(x1 - x2,x2 - x3) < 0);
      } else
      {
        assert(false);
      }
    }
    /*x1<=x2<=x3 && x2-x1<x3-x2 || x3<=x2<=x1 && x1-x2<x2-x3*/
    x4 = x2 + det_offset3(x3 - x2); /* use offset c */
    if (f(x2) <= f(x4))
    { /* f(x_4), case 1 */
      x3 = x1; x1 = x4;
      /* now x3 <= x2 <= x1 and x1 - x2 < x2 - x3 */
    } else
    { /* f(x_4), case 2 */
      x1 = x2; x2 = x4;
      /* now x1 <= x2 <= x3 and x2 - x1 < x3 - x2 */
    }
  }
  printf("iterations=%lu\n",iter);
  return (x1 + x3)/2.0; /* xmin is halfway between x1 and x3*/
}

typedef double (*DDfun)(double);

double gsearchNR(DDfun f,const Tripletofdoubles *td)
{
  unsigned long k;
  double oldleft, x1, x2, x3, fval1, fval2;

  /*printf("start=%e,  %e,  %e\n",td->x1,td->x2,td->x3);*/
  assert(td->x1 < td->x2 && td->x2 < td->x3);
  assert(f(td->x1) > f(td->x2) && f(td->x2) < f(td->x3));

  oldleft = td->x1;
  x3 = td->x3;
  if (fabs(td->x3 - td->x2) > fabs(td->x2 - td->x1))
  { /* x2..x3 is larger than x1..x2 */
    x1 = td->x2;
    x2 = td->x2 + det_offset3(td->x3 - td->x2);
  } else
  { /* x2..x3 is smaller than x1..x2 */
    x2 = td->x2;
    x1 = td->x2 - det_offset3(td->x2 - td->x1);
  }
  fval1 = f(x1);
  fval2 = f(x2);
  for (k=0; fabs(x3 - oldleft) > TOL*(fabs(x1)+fabs(x2)); k++)
  {
    /*printf("x3-x1=%e,x2+x3=%e\n",fabs(x3 - oldleft),fabs(x1) + fabs(x2));
    printf("k=%lu, |a-b|=%e\n", k, fabs(x3 - oldleft));*/
    if (fval2 < fval1)
    {
      oldleft = x1;
      x1 = x2;
      x2 = x1 + det_offset3(x3 - x1);
      fval1 = fval2;
      fval2 = f(x2);
    } else
    {
      x3 = x2;
      x2 = x1;
      x1 = x2 + det_offset3(oldleft - x2);
      fval2 = fval1;
      fval1 = f(x1);
    }
  }
  return (fval1 < fval2) ? x1 : x2;
}

void exponentialsearch(Tripletofdoubles *result,DDfun f,double start)
{
  const double g = GOLD, rg1 = 2.0 - g;
  double a = start, b = start + 1.0, c = start + 1.0 + g, x, fa, fb, fc;

  while (true)
  {
    printf("a=%e,b=%e,c=%e\n",a,b,c);
    assert((c - b)/(b - a) == g);
    fa = f(a);
    fb = f(b);
    fc = f(c);
    if (fa <= fb && fb <= fc)
    {
      x = rg1 * (b - a) + b;
      c = b;
      b = a;
      a = x;
      if (f(x) <= fa)
      {
        break;
      }
    } else
    {
      if (fa >= fb && fb >= fc)
      {
        x = rg1 * (c - b) + c;
        a = b;
        b = c;
        c = x;
        if (f(x) >= fc)
        {
          break;
        }
      } else
      {
        break;
      }
    }
  }
  result->x1 = a;
  result->x2 = b;
  result->x3 = c;
}

static double samplefun(double x)
{
  return ((4.0 * pow(x,3.0) - 6.0 * pow(x,2.0) + 1) *
          sqrt(x+1.0))/(3.0 - x);
}

void findbestvalue(void)
{
  double xmin, mid, bestmid = 0.0, bestxmin = 0.0, bestf = 10.0;

  for (mid = 0.0000; mid < 0.57; mid+=0.001)
  {
    xmin = gsearch(samplefun,0.0,mid,1.5);
    if (double_compare(samplefun(xmin),bestf) < 0)
    {
      bestf = samplefun(xmin);
      bestxmin = xmin;
      bestmid = mid;
    }
  }
  printf("bestmid=%.10e,f(xmin=%.20f)=%.20f\n",
         bestmid,bestxmin,samplefun(bestxmin));
}

int main(void)
{
  double xmin;
  xmin = gsearch(samplefun,0.2,1.0,1.5);
  printf("f(xmin=%.20f)=%.20f\n",xmin,samplefun(xmin));
  /*
  printf("gold=%.6f,1-gold=%.6f\n",GOLD,1.0 - GOLD);
  {
    float astart;
    Tripletofdoubles start;
    start.x1 = 0.0;
    start.x2 = 0.5;
    start.x3 = 1.5;
    xmin = gsearchNR(samplefun,&start);
    printf("gsearchNR: fmin(xmin=%.20f)=%.20f\n",xmin,samplefun(xmin));
    start.x1 = 0.0;
    start.x2 = 0.5;
    start.x3 = 1.5;
    printf("fmin(xmin=%e)=%e\n",xmin,samplefun(xmin));
    if (argc != 2 || sscanf(argv[1],"%f",&astart) != 1)
    {
      fprintf(stderr,"Usage: %s <startvalue (float)>\n",argv[0]);
      return EXIT_FAILURE;
    }
    printf("astart=%e\n",astart);
    exponentialsearch(&start,samplefun,astart);
    xmin = gsearchNR(samplefun,&start);
    printf("fmin(xmin=%e)=%e\n",xmin,samplefun(xmin));
  }
  */
  return EXIT_SUCCESS;
}
