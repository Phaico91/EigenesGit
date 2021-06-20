#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct
{
  double real, imag;
} Complex;

static unsigned long mandelbrot_iter (const Complex c,
                                      unsigned long nmax)
{
  Complex z = c;
  unsigned long n;

  for (n = 0; n < nmax; n++)
  {
    const double a = z.real,
                 b = z.imag;
    if (a * a + b * b > 4.0) /* |z| > 2 */
    { /* equiv. to sqrt(a * a + b * b) > 2.0 */
      return n;
    }
    z.real = a * a - b * b + c.real; /* z = z * z + c */
    z.imag = 2.0 * a * b + c.imag;
  }
  return nmax;
}

typedef uint8_t Matrixbasetype;
#define FormatMatrixbasetype "%u"
const unsigned long maxmatrixvalue
  = (1UL << (CHAR_BIT * sizeof(Matrixbasetype))) - 1;

static Matrixbasetype **mandelbrot_set (bool bw,
                                        unsigned long width,
                                        unsigned long height,
                                        unsigned long nmax)
{
  const double xmin = -2.0, xmax = +0.5, ymin = -1.25, ymax = +1.25,
               dx = (xmax - xmin)/width, /* step size on x */
               dy = (ymax - ymin)/height; /* step size on y */
  double x, y; /* current x and y-value in plane */
  unsigned long i, j, miter;
  Matrixbasetype **matrix = malloc (width * sizeof *matrix);

  assert(matrix != NULL);
  matrix[0] = malloc (width * height * sizeof **matrix); /* data */
  assert(nmax <= maxmatrixvalue && matrix[0] != NULL);
  for (i = 1; i < width; i++)
  { /* set column ptrs */
    matrix[i] = matrix[i-1] + height;
  }
  for (i = 0, x = xmin; i < width; i++, x += dx)
  {
    Complex c;
    c.real = x; /* x value becomes real part of c */
    for (j = 0, y = ymin; j < height; j++, y += dy)
    {
      c.imag = y; /* y value becomes imaginary part of c */
      miter = mandelbrot_iter(c, nmax);
      if (bw)
      {
        matrix[i][j] = miter == nmax ? 1 : 0;
      } else
      {
        assert(miter == nmax || miter + 1 <= maxmatrixvalue);
        matrix[i][j]
          = miter == nmax ? 0 : (Matrixbasetype) (miter + 1);
      }
    }
  }
  return matrix;
}

static void mandelbrot_showmatrix(const Matrixbasetype * 
                                    const *matrix,
                                  unsigned long width,
                                  unsigned long height)
{
  unsigned long i, j;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      printf(FormatMatrixbasetype "%c",
             matrix[i][j],j < height - 1 ? ' ' : '\n');
}

int main(int argc,char *argv[])
{
  bool bw;
  const unsigned long mindpi = 72;
  unsigned long dpi, width, height;
  unsigned long nmax = 0;
  long readlong1, readlong2;
  Matrixbasetype **matrix;

  if (argc != 4 ||
      (strcmp(argv[1],"bw") != 0 && strcmp(argv[1],"color") != 0) ||
      sscanf(argv[2],"%ld",&readlong1) != 1 ||
      readlong1 < (long) mindpi ||
      sscanf(argv[3],"%ld",&readlong2) != 1 ||
      readlong2 <= 0 ||
      readlong2 > maxmatrixvalue)
  {
    fprintf(stderr,"Usage: %s bw|color <dpi>=%lu> <nmax<=%lu>\n",
            argv[0],mindpi,maxmatrixvalue);
    return EXIT_FAILURE;
  }
  bw = strcmp(argv[1],"bw") == 0 ? true : false;
  nmax = (unsigned long) readlong1;
  dpi = (unsigned long) readlong2;
  width = 10 * dpi; /* units of inches */
  height = 10 * dpi; /* units of inches */
  matrix = mandelbrot_set(bw, width, height, nmax);
  mandelbrot_showmatrix((const Matrixbasetype *const *) matrix,width,height);
  free(matrix[0]);
  free(matrix);
  return EXIT_SUCCESS;
}
