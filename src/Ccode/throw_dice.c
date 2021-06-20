/* This is a slightly modified version of the
   program from figure 9.5 of the book of A. Moffat */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

typedef struct
{
  size_t cash, throws;
} Score; /* current state of game */

static Score play_single_game(bool silent,size_t start_cash,
                              size_t threshold)
{
  Score score;
  score.cash = start_cash;
  for (score.throws = 1;
       score.cash > 0 && score.cash <= threshold;
       score.throws++)
  {
    size_t dice1, dice2;

    score.cash--;  /* player pays one unit */
    dice1 = 1 + drand48() * 6; /* simulation: roll 1st dice */
    dice2 = 1 + drand48() * 6; /* simulation: roll 2nd dice */
    if (dice1 + dice2 == 12)
    {
      score.cash += 6;
    } else
    {
      if (dice1 + dice2 >= 8)
      {
        score.cash += 2;
      }
    }
    if (!silent)
      printf("%3lu throw%c |%*s*\n",score.throws,
             score.throws == 1 ? ' ' : 's',
             (int) score.cash,"");
             /* show * with indendation of width = cash */
  }
  return score;
}

static void showresult(const char *key,
                       size_t numevents, /* win/loose */
                       size_t numgames,
                       size_t totalthrows)
{
  printf("%5s: %lu times (prob: %.2f), "
         "average number of throws: %.2f\n",
         key,numevents,(double) numevents/numgames,
         (double) totalthrows/numevents);
}

static void play_many_games(size_t start_cash,
                            size_t threshold,
                            size_t numgames)
{
  size_t g, loose = 0, loose_totalthrows = 0,
                       win_totalthrows = 0;
  for (g = 0; g < numgames; g++)
  {
    Score score = play_single_game(true,start_cash,threshold);
    if (score.cash == 0)
    {
      loose++;
      loose_totalthrows += score.throws;
    } else
      win_totalthrows += score.throws;
  }
  showresult("win",  numgames-loose,numgames,win_totalthrows);
  showresult("loose",loose,  numgames,loose_totalthrows);
}

int main(int argc,char *argv[])
{
  long start_cash, threshold, numgames = 0;

  if (argc < 3 || argc > 4 ||
      sscanf(argv[1],"%ld",&start_cash) != 1 ||
      start_cash < 1 ||
      sscanf(argv[2],"%ld",&threshold) != 1 ||
      threshold < start_cash ||
      (argc == 4 && (sscanf(argv[3],"%ld",&numgames) != 1 ||
                     numgames < 1)))
  {
    fprintf(stderr,"Usage: %s <start_cash> <threshold> "
                   "[numgames > 0]\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  srand48(time(NULL)); /* initialze random number generator,
                          use current time as seed
                          requires #include <time.h> */
  if (numgames == 0) /* single round, output protocol */
  {
    Score score = play_single_game(false,(size_t) start_cash,
                                   (size_t) threshold);
    if (score.cash == 0)
    {
      printf("lost all\n");
    } else
    {
      printf("won %lu\n",score.cash);
    }
  } else
  { /* play <numgames> rounds, show final statistics */
    play_many_games((size_t) start_cash,
                    (size_t) threshold,
                    (size_t) numgames);
  }
  return EXIT_SUCCESS;
}
