/*
  This example program illustrates the use of mutex variables
  in a threads program. The main data is made available to all threads
  through a pointer to shareddata structure of the main program.
  Each thread works on a different part of the data. The main thread waits
  for all the threads to complete their computations, and then it prints
  the resulting sum.
  SOURCE: Vijay Sonnad, IBM
  Modified by Stefan Kurtz
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/*
  The following structure contains the necessary information
  to allow the function "dotprod" to access its input data and
  place its output into the structure.  This structure is
  unchanged from the sequential version.
*/

/* Define globally accessible variables and a mutex */

typedef struct
{
  double *a, *b, sum;
  unsigned long veclen;
  pthread_mutex_t mutexsum;
} Shareddata;

typedef struct
{
  Shareddata *sd_ptr;
  unsigned long threadnum;
  pthread_t thread_id;
} Threadinfo;

/*
  The function dotprod is activated when the thread is created.
  As before, all input to this routine is obtained from a structure
  of type Shareddata and all output from this function is written into
  this structure. The benefit of this approach is apparent for the
  multi-threaded program: when a thread is created we pass a single
  argument to the activated function.
*/

static void *dotprod(void *vthreadinfo)
{
  Threadinfo *threadinfo = (Threadinfo *) vthreadinfo;
  unsigned long idx, start, end;
  double threadsum = 0.0;

  start = threadinfo->threadnum * threadinfo->sd_ptr->veclen;
  end = start + threadinfo->sd_ptr->veclen;
  for (idx = start; idx<end ; idx++)
    threadsum += (threadinfo->sd_ptr->a[idx] *
                  threadinfo->sd_ptr->b[idx]);
  pthread_mutex_lock (&threadinfo->sd_ptr->mutexsum);
  threadinfo->sd_ptr->sum += threadsum;
  printf("thread %lu [%7lu,%7lu]: threadsum=%.2f sum=%.2f\n",
          threadinfo->threadnum,start,end,threadsum,
          threadinfo->sd_ptr->sum);
  pthread_mutex_unlock (&threadinfo->sd_ptr->mutexsum);
  return NULL;
}

/*
  The main program creates threads which do all the work and then
  print out result upon completion. Before creating the threads,
  The input data is created. Since all threads update a shared structure, we
  need a mutex for mutual exclusion. The main thread needs to wait for
  all threads to complete, it waits for each one of the threads. We specify
  a thread attribute value that allow the main thread to join with the
  threads it creates. Note also that we free up handles  when they are
  no longer needed.
*/

static Shareddata *shareddata_new(unsigned long numthreads,
                                  unsigned long veclen)
{
  unsigned long idx;
  Shareddata *sd = malloc(sizeof *sd);

  assert(sd != NULL);
  sd->a = malloc (numthreads * veclen * sizeof *sd->a);
  sd->b = malloc (numthreads * veclen * sizeof *sd->b);
  assert(sd->a != NULL && sd->b != NULL);
  srand48(366292341);
  for (idx = 0; idx < veclen * numthreads; idx++) {
    sd->a[idx] = drand48();
    sd->b[idx] = drand48();
  }
  sd->veclen = veclen;
  sd->sum = 0;
  pthread_mutex_init(&sd->mutexsum, NULL);
  return sd;
}

static void shareddata_delete(Shareddata *sd)
{
  if (sd != NULL)
  {
    free(sd->a);
    free(sd->b);
    pthread_mutex_destroy(&sd->mutexsum);
    free(sd);
  }
}

int main(void) {
  unsigned long idx;
  const unsigned long numthreads = 4, veclen = 100000;
  Threadinfo threadinfo[numthreads];
  Shareddata *shareddata;

  shareddata = shareddata_new(numthreads,veclen);
  for (idx=0; idx<numthreads; idx++) {
    threadinfo[idx].threadnum = idx;
    threadinfo[idx].sd_ptr = shareddata;
    pthread_create(&threadinfo[idx].thread_id, NULL, dotprod,
                   (void *) &threadinfo[idx]);
  }
  for (idx = 0; idx < numthreads; idx++)
    pthread_join(threadinfo[idx].thread_id, NULL);
  printf ("sum = %.2f \n", shareddata->sum);
  shareddata_delete(shareddata);
  return EXIT_SUCCESS;
}
