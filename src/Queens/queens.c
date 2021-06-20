#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

static void usage(const char *progname,bool show_options)
{
  const char *optionsmsg =
    "  -d\tdisplay placement of queens on chessboard\n"
    "  -i\tuse iterative implementation\n"
    "  -h\tshow this usage message";
  fprintf(stderr, "Usage: %s [options] number\ncount and possibly "
          "display soliutions of the n-Queens problem\n%s\n",
          progname,
          show_options ? optionsmsg : "Use -h for more information.");
}

typedef struct
{
  size_t size,  /* number of queens, rows, columns */
         solutions; /* counter for found solutions */
  int *columns,
      valid;
  bool *in_col, /* in_col[j] iff a queen in column j */
       *on_a_diag, /* on_a_diag[q] iff a queen on antidiag q */
       *on_diag, /* on_diag[d] iff a queen on diag d */
       display;
} Board;

static int a_diag_idx(int i,int j)
{
  return i + j;
}

static int diag_idx(int i,int j)
{
  return j - i;
}

static bool is_placement_possible(const Board *bd,int i,int j)
{
  return !bd->in_col[j] &&
         !bd->on_a_diag[a_diag_idx(i,j)] &&
         !bd->on_diag[diag_idx(i,j)];
}

static void do_placement(Board *bd,int i,int j) {
  bd->in_col[j] = bd->on_a_diag[a_diag_idx(i,j)]
                = bd->on_diag[diag_idx(i,j)] = true;
  bd->columns[i] = j;
  bd->valid = i + 1;
}

static void undo_placement(Board *bd,int i,int j) {
  bd->in_col[j] = bd->on_a_diag[a_diag_idx(i,j)]
                = bd->on_diag[diag_idx(i,j)] = false;
  bd->columns[i] = (int) bd->size; /* undefined */
  bd->valid = i;
}

static void display_board(const Board *bd)
{
  int i;

  for (i = 0; i < bd->size; i++)
  {
    int j;

    assert(bd->columns[i] < bd->size);
    for (j = 0; j < bd->size; j++)
    {
      printf("%c%c",j == bd->columns[i] ? 'Q' : '.',
                    j < bd->size - 1 ? ' ' : '\n');
    }
  }
  for (i=0; i < 2 * bd->size; i++) /* ----- line */
    printf("%c",i < 2 * bd->size - 1 ? '-' : '\n');
}

static void queens_rec(int i,Board *bd)
{
  if (i < bd->size)
  {
    int j;
    for (j = 0; j < bd->size; j++)
    {
      if (is_placement_possible(bd,i,j))
      {
        do_placement(bd,i,j);
        queens_rec(i+1,bd);
        undo_placement(bd,i,j);
      }
    }
  } else
  {
    if (bd->display) display_board(bd);
    bd->solutions++;
  }
}

static void queens_itrv(Board *bd)
{
  int i = 0, j = 0;

  while (true)
  {
    while (j < bd->size)
    {
      if (is_placement_possible(bd,i,j))
      {
        do_placement(bd,i,j);
        i++;
        j = 0;
        break;
      }
      j++;
    }
    if (j == bd->size || i == bd->size)
    {
      int prev_column;

      if (i == bd->size)
      {
        if (bd->display)
        {
          display_board(bd);
        }
        bd->solutions++;
#define WITH_OPTIMIZATION
#ifdef WITH_OPTIMIZATION
        prev_column = bd->columns[bd->valid-1];
        undo_placement(bd,bd->valid-1,prev_column);
        i--;
#endif
      }
      if (bd->valid == 0)
      {
        break;
      }
      prev_column = bd->columns[bd->valid-1];
      undo_placement(bd,bd->valid-1,prev_column);
      i--;
      j = 1 + prev_column;
    }
  }
}

static size_t queens(size_t size,bool display,bool iterative)
{
  size_t row;
  Board bd;

  bd.size = size;
  bd.solutions = 0;
  bd.in_col = calloc(5 * bd.size - 2,sizeof *bd.in_col);
  bd.on_a_diag = bd.in_col + bd.size;
  bd.on_diag = bd.on_a_diag + 2 * bd.size - 1 + bd.size - 1;

  bd.columns = malloc(sizeof *bd.columns * bd.size);
  for (row = 0; row < bd.size; row++)
  {
    bd.columns[row] = bd.size;
  }
  bd.display = display;
  bd.valid = 0;
  if (iterative)
    queens_itrv(&bd); /* call iterative method. not shown */
  else
    queens_rec(0,&bd); /* call recursive method, see below */
  free(bd.in_col); /* only one calloc => only one free */
  free(bd.columns);
  return bd.solutions;
}

typedef struct
{
  bool display,
       iterative;
  size_t size;
} Options;

static Options *options_new(int argc, char *const *argv)
{
  int opt;
  Options *options = malloc(sizeof *options);
  bool haserr = false;

  assert(options != NULL);
  options->display = false;
  options->iterative = false;
  options->size = 0;
  while ((opt = getopt(argc, argv, "dih")) != -1)
  {
    switch ((char) opt)
    {
      case 'd':
        options->display = true;
        break;
      case 'i':
        options->iterative = true;
        break;
      case 'h':
        usage(argv[0], true);
        haserr = true;
        break;
      default:
        assert((char) opt == '?');
        usage(argv[0], false);
        haserr = true;
        break;
    }
  }
  if (!haserr && optind < argc - 1)
  {
    fprintf(stderr,"%s: superfluous argument %s\n",argv[0],argv[optind]);
    haserr = true;
  }
  if (!haserr && optind > argc - 1)
  {
    fprintf(stderr,"%s: missing argument\n",argv[0]);
    haserr = true;
  }
  if (!haserr)
  {
    int readint;

    if (sscanf(argv[optind],"%d",&readint) != 1 || readint < 1)
    {
      fprintf(stderr,"%s: last parameter must be positive integert\n",argv[0]);
      haserr = true;
    }
    options->size = (size_t) readint;
  }
  if (haserr)
  {
    free(options);
    return NULL;
  }
  return options;
}

static void options_delete(Options *options)
{
  if (options != NULL)
  {
    free(options);
  }
}

int main(int argc,char *argv[])
{
  Options *options = options_new(argc,(char *const *) argv);

  if (options == NULL)
  {
    return EXIT_FAILURE;
  }
  printf("%lu\t%lu\n",options->size,
                      queens(options->size,options->display,
                             options->iterative));
  options_delete(options);
  return EXIT_SUCCESS;
}
