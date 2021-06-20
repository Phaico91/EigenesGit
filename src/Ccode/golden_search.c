#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <float.h>
#include <unistd.h>
#include "double_compare.h"

#define GOLD  ((sqrt(5.0) - 1.0)/2.0) /* 0.6180339887 */

#define TOL 1.4901161028591027e-08 /* sqrt(epsilon) */
static bool sufficiently_small(double l,double r)
{
  assert (l <= r);
  return (r - l) < TOL ? true : false;
}

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) < (Y) ? (Y) : (X))

static double gsearch(double (*f)(double),double varphi,
                      double l,double r)
{
  unsigned long iter;

  for (iter = 0; !sufficiently_small(l,r); iter++)
  {
    double b = varphi * (r - l),
           p = r - b, q = l + b;

    if (f(p) < f(q))
    {
      r = q; /* case 1 */
    } else
    {
      l = p; /* case 2 */
    }
  }
  printf("iterations=%lu\n",iter);
  return l + (r - l)/2.0; /* xmin is halfway between l and r*/
}

static double gsearch_fast(double (*f)(double),double varphi,
                           double l,double r)
{
  unsigned long iter;
  int previouscase = 0;
  double fp = DBL_MAX, fq = DBL_MAX;

  for (iter = 0; !sufficiently_small(l,r); iter++)
  {
    double b = varphi * (r - l),
           p =  r - b, q =  l + b;

    /*printf("%f %f %f %f\n",l,p,q,r); */
    if (previouscase == 1)
    {
      fq = fp;
      fp = f(p);
    } else
    {
      if (previouscase == 2)
      {
        fp = fq;
        fq = f(q);
      } else
      {
        fp = f(p);
        fq = f(q);
      }
    }
    if (fp < fq)
    {
      previouscase = 1;
      r = q; /* case 1 */
    } else
    {
      previouscase = 2;
      l = p; /* case 2 */
    }
  }
  printf("iterations=%lu\n",iter);
  return l + (r - l)/2.0; /* xmin is halfway between l and r*/
}

static double func0(double x)
{
  return ((4.0 * pow(x,3.0) - 6.0 * pow(x,2.0) + 1) *
          sqrt(x+1.0))/(3.0 - x);
}

/* see the functions at https://en.wikipedia.org/wiki/Quadratic_function */

double func1(double x)
{
  return x * (x-3);
}

double func2(double x)
{
  return x * (x+2);
}

double func3(double x)
{
  return (x - 5.0) * (x - 5.0);
}

typedef double (*DDfunc)(double);

static void usage(const char *progname)
{
  fprintf(stderr,"Usage: %s options\n"
          "    -s <float> specify split factor varphi in the range from\n"
          "               0.5 to 1.0 (excluding these values)\n"
          "               default is %f\n"
          "    -f         run fast implementation minimizing number of\n"
          "               function calls\n"
          "    -h         show help\n",
         progname,GOLD);
}

typedef struct
{
  bool fast_eval;
  DDfunc function_ptr;
  double varphi;
} Goldoptions;

static void options_parse(Goldoptions *go,int argc, char * const argv[])
{
  const char *optioncharacters = "sfhn";
  go->fast_eval = false;
  go->varphi = GOLD;
  go->function_ptr = func0;

  if (argc > 1)
  {
    while (true)
    {
      int opt = getopt(argc, argv, optioncharacters);
      if (opt == -1)
      {
        break;
      }
      switch ((char) opt)
      {
        case 's':
        case 'n':
          if (optind >= argc)
          {
            fprintf(stderr,"%s: missing argument to option -%c\n",argv[0],
                      (char) opt);
            usage(argv[0]);
            exit(EXIT_FAILURE);
          } else
          {
            if (opt == 's')
            {
              if (sscanf(argv[optind],"%lf",&go->varphi) != 1 ||
                  go->varphi <= 0.5 || go->varphi >= 1.0)
              {
                fprintf(stderr,"%s: argument to option -%c must be floating "
                               " point value in the range from 0.5 to 1.0 "
                               "(excluding these values)\n",argv[0],opt);
                usage(argv[0]);
                exit(EXIT_FAILURE);
              }
            } else
            {
              long func_num;
              static DDfunc func_table[] =
              {
                func0,
                func1,
                func2,
                func3
              };
              const unsigned long numoffunctions
                = sizeof func_table/sizeof func_table[0];
              if (sscanf(argv[optind],"%ld",&func_num) != 1 ||
                  func_num < 0 || func_num > numoffunctions-1)
              {
                fprintf(stderr,"%s: argument to option -%c must be integer "
                               " in the range from 0 to %lu (default 0)",
                               argv[0],opt,numoffunctions - 1);
                usage(argv[0]);
                exit(EXIT_FAILURE);
              }
              go->function_ptr = func_table[func_num];
            }
          }
          optind++;
          break;
        case 'f':
          go->fast_eval = true;
          break;

        case 'h':
          usage(argv[0]);
          exit(EXIT_SUCCESS);
        default:
          fprintf(stderr,"%s: illegal option -%c\n",argv[0],(char) opt);
          exit(EXIT_FAILURE);
      }
    }
  }
  if (optind != argc)
  {
    fprintf(stderr,"%s: superflous options\n",argv[0]);
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
}

int main(int argc,char *argv[])
{
  double xmin;
  double lvalue = 0.2, rvalue = 1.5;
  /*double lvalue = 0, rvalue = 10;*/
  Goldoptions go;

  options_parse(&go,argc,argv);
  if (go.fast_eval)
  {
    xmin = gsearch_fast(go.function_ptr,go.varphi,lvalue,rvalue);
  } else
  {
    xmin = gsearch(go.function_ptr,go.varphi,lvalue,rvalue);
  }
  printf("f(xmin=%.20f)=%.20f\n",xmin,go.function_ptr(xmin));
  return EXIT_SUCCESS;
}
