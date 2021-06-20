#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *print_msg(void *ptr)
{
  printf("%s\n", (char *) ptr);
  return NULL;
}

int main(void)
{
  const char *msg1 = "Thread 1";
  const char *msg2 = "Thread 2";
  pthread_t tid1, tid2;

  /* Create independent threads each of which will
     execute function */
  pthread_create(&tid1, NULL, print_msg, (void *) msg1);
  pthread_create(&tid2, NULL, print_msg, (void *) msg2);
  /* Wait until threads are complete before main continues.
     Unless we wait we have the risk of executing an exit
     which will terminate the process and all threads
     before the threads have completed. */

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("Thread 1 finished\n");
  printf("Thread 2 finished\n");
  exit(EXIT_SUCCESS);
}
