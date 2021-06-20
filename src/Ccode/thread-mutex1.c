#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
  pthread_mutex_t mutex1;
  unsigned long counter;
} Sharedvars;

static void *increment(void *data)
{
  Sharedvars *sv = (Sharedvars *) data;

  pthread_mutex_lock(&sv->mutex1);
  sv->counter++;
  pthread_mutex_unlock(&sv->mutex1);
  return NULL;
}

int main(void)
{
  const unsigned long numthreads = 10;
  unsigned long t;
  pthread_t threadid_tab[numthreads];
  Sharedvars sv = {PTHREAD_MUTEX_INITIALIZER,0};

  /* Create independent threads each of which
     will execute increment */

  for (t = 0; t < numthreads; t++)
    pthread_create(&threadid_tab[t], NULL, 
                   increment, &sv);

  for (t = 0; t < numthreads; t++)
    pthread_join(threadid_tab[t], NULL);
  printf("counter=%lu\n",sv.counter);
  exit(EXIT_SUCCESS);
}
