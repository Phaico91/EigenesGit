#include <pthread.h>
pthread_mutex_t amutex = PTHREAD_MUTEX_INITIALIZER;
/* or pthread_mutex_init(&amutex, NULL); */
pthread_mutex_lock(amutex);
int pthread_mutex_unlock(amutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
