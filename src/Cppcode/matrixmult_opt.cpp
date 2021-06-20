#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "matrixmult_opt.h"

static void usage(const char *prog,
                  unsigned long default_size,
                  unsigned long default_dc_width_threshold)
{
  fprintf(stderr,"Usage: %s options\n"
          "    -o <op> specify white space separated list of operations:\n"
          "            mm_s = matrix multiplication standard algorithm\n"
          "            mm_t = matrix multiplication after transposition\n"
          "            mm_dc = matrix multiplication (divide/conquer)\n"
          "            mm_v2 = vectorized matrix multiplication using vector\n"
          "                    instructions\n"
          "            mm_tv2 = vectorized matrix multiplication using vector\n"
          "                     instructions after transposition\n"
          "            ma = matrix addition\n"
          "            split = matrix splitting\n"
          "       if more then one of the mm-operations are used then results\n"
          "       are compared for testing\n"
          "    -b show matrix base type\n"
          "    -m <size of m> (default is %lu)\n"
          "    -n <size of n> (default is %lu)\n"
          "    -l <size of l> (default is %lu)\n"
          "    -w <dc width threshold> (default is %lu)\n"
          "    -f <inputfileA> <inputfileB> (specify matrices in file)\n"
          "    -c generate matrix of continuous values\n"
          "    -t specify number of threads for mm_dc\n"
#ifdef __cplusplus
          "    -y specify basetype of matrices: possible choices:\n"
          "       int, float, double, default is double\n"
#endif
          "    -s output matrix\n"
          "    -v verbose output\n",
          prog,
          default_size,
          default_size,
          default_size,
          default_dc_width_threshold);
}

#define USAGE usage(argv[0],default_size,default_dc_width_threshold);\
              matrix_mult_options_delete(options);\
              exit(EXIT_FAILURE)

struct MMoptions
{
  bool show, verbose, continuous, showmatrixbasetype;
  unsigned long rowsA, columnsA, columnsB, dc_width_threshold;
  char *inputfileA, *inputfileB;
  bool compare;
  unsigned int matrixopbits;
  unsigned long num_threads;
#ifdef __cplusplus
  const char *typestring;
#endif
};

static unsigned int matrixop2bit(int matrixop)
{
  assert(matrixop <= (int) OP_split);
  return 1U << matrixop;
}

static const char *opnames[] =
{
  "multiplication standard",
  "multiplication standard after transposition",
  "multiplication divide/conquer",
  "multiplication vectorize using vector instructions",
  "multiplication vectorize using vector instructions and tranposition",
  "addition",
  "split"
};

unsigned int matrix_mult_options_last_op(void)
{
  return (unsigned int) OP_multiplication_transposed_vector;
}

bool matrix_mult_options_showmatrixbasetype(const MMoptions *options)
{
  assert(options != NULL);
  return options->showmatrixbasetype;
}

#ifdef __cplusplus
const char *matrix_mult_options_typestring(const MMoptions *options)
{
  assert(options != NULL);
  return options->typestring;
}
#endif

void matrix_mult_options_show_options(const MMoptions *options)
{
  int mop;
  assert(options != NULL);
  if (!options->verbose)
  {
    return;
  }
  printf("rowsA=%lu\n",options->rowsA);
  printf("columnsA=%lu\n",options->columnsA);
  printf("columnsB=%lu\n",options->columnsB);
  printf("coninuous=%s\n",options->continuous ? "true" : "false");
  printf("verbose=%s\n",options->verbose ? "true" : "false");
  printf("show=%s\n",options->show ? "true" : "false");
  printf("dc_thresold=%lu\n",options->dc_width_threshold);
#ifdef __cplusplus
  printf("type=%s\n",options->typestring);
#endif
  if (options->inputfileA != NULL)
  {
    printf("inputfileA=%s\n",options->inputfileA);
  }
  if (options->inputfileB != NULL)
  {
    printf("inputfileB=%s\n",options->inputfileB);
  }
  for (mop = 0; mop <= (int) OP_split; mop++)
  {
    if (options->matrixopbits & matrixop2bit(mop))
    {
      printf("matrixop=%s\n",opnames[mop]);
    }
  }
}

const char *matrix_mult_options_inputfileA(const MMoptions *options)
{
  assert(options != NULL);
  return options->inputfileA;
}

const char *matrix_mult_options_inputfileB(const MMoptions *options)
{
  assert(options != NULL);
  return options->inputfileB;
}

bool matrix_mult_options_continuous(const MMoptions *options)
{
  assert(options != NULL);
  return options->continuous;
}

unsigned long matrix_mult_options_rowsA(const MMoptions *options)
{
  assert(options != NULL);
  return options->rowsA;
}

unsigned long matrix_mult_options_columnsA(const MMoptions *options)
{
  assert(options != NULL);
  return options->columnsA;
}

unsigned long matrix_mult_options_columnsB(const MMoptions *options)
{
  assert(options != NULL);
  return options->columnsB;
}

bool matrix_mult_options_show(const MMoptions *options)
{
  assert(options != NULL);
  return options->show;
}

unsigned long matrix_mult_options_num_threads(const MMoptions *options)
{
  assert(options != NULL);
  return options->num_threads;
}

unsigned long matrix_mult_options_dc_width_threshold(const MMoptions *options)
{
  assert(options != NULL);
  return options->dc_width_threshold;
}

bool matrix_mult_options_only_split(const MMoptions *options)
{
  assert(options != NULL);
  return options->matrixopbits == matrixop2bit(OP_split) ? true : false;
}

bool matrix_mult_options_split(const MMoptions *options)
{
  assert(options != NULL);
  return (options->matrixopbits & matrixop2bit(OP_split)) ? true : false;
}

bool matrix_mult_options_addition(const MMoptions *options)
{
  assert(options != NULL);
  return (options->matrixopbits & matrixop2bit(OP_addition)) ? true : false;
}

bool matrix_mult_options_multiplication_standard(const MMoptions *options)
{
  assert(options != NULL);
  return (options->matrixopbits & matrixop2bit(OP_multiplication_standard)) 
            ? true : false;
}

bool matrix_mult_options_multiplication_transposed(const MMoptions *options)
{
  assert(options != NULL);
  return (options->matrixopbits & matrixop2bit(OP_multiplication_transposed)) 
            ? true : false;
}

bool matrix_mult_options_multiplication_dc(const MMoptions *options)
{
  assert(options != NULL);
  return (options->matrixopbits & matrixop2bit(OP_multiplication_dc)) 
            ? true : false;
}

bool matrix_mult_options_multiplication_vector(const MMoptions *options)
{
  assert(options != NULL);
  return (options->matrixopbits & matrixop2bit(OP_multiplication_vector)) 
            ? true : false;
}

bool matrix_mult_options_multiplication_transposed_vector(
                      const MMoptions *options)
{
  assert(options != NULL);
  return (options->matrixopbits & 
          matrixop2bit(OP_multiplication_transposed_vector))
            ? true : false;
}

static int extractmatrixoperation(const char *opstring)
{
  if (strcmp(opstring,"mm_s") == 0)
  {
    return (int) OP_multiplication_standard;
  }
  if (strcmp(opstring,"mm_t") == 0)
  {
    return (int) OP_multiplication_transposed;
  }
  if (strcmp(opstring,"mm_dc") == 0)
  {
    return (int) OP_multiplication_dc;
  }
  if (strcmp(opstring,"mm_v2") == 0)
  {
    return (int) OP_multiplication_vector;
  }
  if (strcmp(opstring,"mm_tv2") == 0)
  {
    return (int) OP_multiplication_transposed_vector;
  }
  if (strcmp(opstring,"ma") == 0)
  {
    return (int) OP_addition;
  }
  if (strcmp(opstring,"split") == 0)
  {
    return (int) OP_split;
  }
  return -1;
}

MMoptions *matrix_mult_options_new(int argc,char * const argv[])
{
  int opt;
  long readlong;
  const unsigned long default_size = 15,
                      default_dc_width_threshold = 100;
  bool foundargs = false;
#ifdef __cplusplus
  const char *optioncharacters = "bcfmnlostwvy";
#else
  const char *optioncharacters = "bcfmnlostwv";
#endif
  MMoptions *options = (MMoptions *) malloc(sizeof *options);

  options->show = false;
  options->showmatrixbasetype = false;
  options->verbose = false;
  options->continuous = false;
  options->rowsA = default_size;
  options->columnsA = default_size;
  options->columnsB = default_size;
  options->matrixopbits = 0;
  options->num_threads = 1;
  options->dc_width_threshold = default_dc_width_threshold;
  options->inputfileA = NULL;
  options->inputfileB = NULL;
#ifdef __cplusplus
  options->typestring = "double";
#endif
  if (argc == 1)
  {
    USAGE;
  }
  while (true)
  {
    opt = getopt(argc, argv, optioncharacters);
    if (opt == -1)
    {
      break;
    }
    switch ((char) opt)
    {
      case 's':
        options->show = true;
        break;
      case 'b':
        options->showmatrixbasetype = true;
        break;
      case 'v':
        options->verbose = true;
        break;
      case 'c':
        options->continuous = true;
        break;
      case 'f':
        if (optind + 1 >= argc)
        {
          fprintf(stderr,"%s: missing arguments to option -%c\n",argv[0],
                    (char) opt);
          USAGE;
        } else
        {
          options->inputfileA = strdup(argv[optind]);
          options->inputfileB = strdup(argv[optind+1]);
        }
        optind+=2;
        break;
      case 'o':
        foundargs = false;
        while (optind < argc && argv[optind][0] != '-')
        {
          const int mop = extractmatrixoperation(argv[optind]);

          if (mop == -1)
          {
            fprintf(stderr,"%s: illegal argument to option -%c"
                           "\npossible values are mm, ma, split\n",argv[0],
                    (char) opt);
            USAGE;
          }
          foundargs = true;
          assert(mop <= OP_split);
          options->matrixopbits |= matrixop2bit(mop);
          optind++;
        }
        if (!foundargs)
        {
          fprintf(stderr,"%s: missing argument to option -%c\n",argv[0],
                    (char) opt);
          USAGE;
        }
        break;
#ifdef __cplusplus
      case 'y':
        if (optind >= argc)
        {
          fprintf(stderr,"%s: missing argument to option -%c\n",argv[0],
                  (char) opt);
          USAGE;
        }
        options->typestring = argv[optind];
        if (strcmp(options->typestring,"double") != 0 &&
            strcmp(options->typestring,"float") != 0 &&
            strcmp(options->typestring,"int") != 0)
        {
          fprintf(stderr,"%s: argument to option -%c must be double, int or "
                         "float\n",argv[0],(char) opt);
          USAGE;
        }
        optind++;
        break;
#endif
      default:
        if (strchr(optioncharacters,opt) == NULL)
        {
          fprintf(stderr,"%s: illegal argument to option -%c\n",argv[0],
                  (char) opt);
          USAGE;
        } else
        {
          if (optind >= argc || sscanf(argv[optind],"%ld",&readlong) != 1 ||
              readlong < 1)
          {
            fprintf(stderr,"%s: option -%c needs positive integer argument\n",
                    argv[0],(char) opt);
            USAGE;
          }
          switch((char) opt)
          {
             case 't':
              options->num_threads = (unsigned long) readlong;
#ifndef __cplusplus
              if (options->num_threads > 2)
              {
                fprintf(stderr,"%s: option -%c needs integer argument <= 2\n",
                        argv[0],(char) opt);
                USAGE;
              }
#endif
              break;
            case 'm':
              options->rowsA = (unsigned long) readlong;
              break;
            case 'n':
              options->columnsA = (unsigned long) readlong;
              break;
            case 'l':
              options->columnsB = (unsigned long) readlong;
              break;
            case 'w':
              options->dc_width_threshold = (unsigned long) readlong;
              break;
          }
          optind++;
        }
    }
  }
  if (optind != argc)
  {
    fprintf(stderr,"%s: superflous options\n",argv[0]);
    USAGE;
  }
  if (options->inputfileA != NULL && options->continuous)
  {
    fprintf(stderr,"%s: options -f and -c cannot be combined\n",argv[0]);
    USAGE;
  }
  return options;
}

void matrix_mult_options_delete(MMoptions *options)
{
  if (options != NULL)
  {
    if (options->inputfileA != NULL)
    {
      free(options->inputfileA);
    }
    if (options->inputfileB != NULL)
    {
      free(options->inputfileB);
    }
    free(options);
  }
}
