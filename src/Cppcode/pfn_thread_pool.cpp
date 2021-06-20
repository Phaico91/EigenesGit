#include <stdlib.h>
#include <assert.h>
#include <thread>
#include <vector>
#ifdef WITH_MUTEX
#include "virtual_queue.hpp"
#else
#include "atomic_queue.hpp"
#endif
#include "pfn_thread_pool.hpp"

static void pfn_thread_apply_thread_func(PfNThreadFunc thread_func,
                                         void *thread_data,
                                         VirtualQueue *vq,
                                         size_t thread_id)
{
  size_t task_num;

  while ((task_num = vq->next_element()) <= vq->last_element())
  {
    thread_func(thread_id, task_num, thread_data);
  }
}

void pfn_run_threaded(size_t num_threads, size_t number_of_tasks,
                      PfNThreadFunc thread_func, void *thread_data) {
  assert(num_threads >= 1 && number_of_tasks > 0);
  if (num_threads > 1) {
    std::vector<std::thread> threads{};
    VirtualQueue vq(number_of_tasks);
    for (size_t thd = 0; thd < num_threads; thd++)
      threads.push_back(std::thread(pfn_thread_apply_thread_func,
                                    thread_func,thread_data,
                                    &vq, thd));
    for (auto &th : threads) th.join(); 
  } else
  {
    for (size_t task_num = 0; task_num < number_of_tasks; task_num++)
      thread_func(0,task_num,thread_data);
  }
}
