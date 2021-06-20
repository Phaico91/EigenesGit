#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUMTHREADS 4
#define N 100
#define MEGEXTRA 100000

typedef struct
{
  pthread_attr_t *attr_ptr;
  pthread_t thread_id;
  int threadnum;
} Threadinfo;

void *dowork(void *vthreadinfo)
{
   double sum = 0, A[N][N];
   int i,j;
   size_t mystacksize;
   Threadinfo *threadinfo = (Threadinfo *) vthreadinfo;

   pthread_attr_getstacksize (threadinfo->attr_ptr, &mystacksize);
   printf("Thread %d: stack size = %lu bytes\n", threadinfo->threadnum,
           mystacksize);
   for (i=0; i<N; i++)
   {
     for (j=0; j<N; j++)
     {
       A[i][j] = ((i*j)/3.452) + (N-i);
     }
   }
   for (i=0; i<N; i++)
   {
     for (j=0; j<N; j++)
     {
       sum += A[i][j];
     }
   }
   printf("sum=%.2f\n",sum);
   pthread_exit(NULL);
}

int main(void)
{
  Threadinfo threadinfo[NUMTHREADS];
  size_t stacksize;
  int rc, t;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_getstacksize (&attr, &stacksize);
  printf("Default stack size = %lu\n", stacksize);
  stacksize = N * N * sizeof (double) + MEGEXTRA;
  printf("Amount of stack needed per thread = %lu\n",stacksize);
  pthread_attr_setstacksize (&attr, stacksize);
  printf("Creating threads with stack size = %lu bytes\n",stacksize);
  pthread_attr_getstacksize (&attr, &stacksize);
  /* the next statement shows that stacksize was not changed */
  printf("check stack size = %lu bytes\n",stacksize);
  for (t=0; t<NUMTHREADS; t++)
  {
    threadinfo[t].threadnum = t;
    threadinfo[t].attr_ptr = &attr;
    rc = pthread_create(&threadinfo[t].thread_id, &attr, dowork,
                        (void *) &threadinfo[t]);
    assert(rc == 0);
  }
  printf("Created %d threads\n", t);
  pthread_attr_destroy(&attr);
  pthread_exit(NULL);
}
