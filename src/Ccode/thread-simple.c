#include <pthread.h>
void *func(void *arg) { /* execute in parallel */ }
pthread_t id; int threadnum = 0;
pthread_create(&id,NULL,
               func,
               &threadnum);
...
pthread_join(id,NULL);
