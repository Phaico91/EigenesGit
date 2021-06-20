#ifndef PFN_THREAD_POOL_HPP
#define PFN_THREAD_POOL_HPP
#include <stdlib.h>

using PfNThreadFunc = void (*)(size_t thread_id,size_t task_num,
                               void *thread_data);

void pfn_run_threaded(size_t num_threads,size_t number_of_tasks,
                      PfNThreadFunc thread_proc,void *thread_data);

#endif
