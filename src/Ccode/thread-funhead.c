int pthread_create(pthread_t *,const pthread_attr_t *,
                   void * (*thread_fun)(void *), void *arg);
errcode = pthread_create(&thread_id, &thread_attribute,
                         thread_fun, &argstruct);
