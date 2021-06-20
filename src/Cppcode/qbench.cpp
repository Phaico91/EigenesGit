#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include "pfn_thread_pool.hpp"
#ifdef WITH_MUTEX
#define RACE_PREVENTION "mutex"
#else
#define RACE_PREVENTION "atomic variable"
#endif

static void sum_elements(size_t thread_id,size_t elem,void *data)
{
  size_t *thread_sum = (size_t *) data;
  thread_sum[thread_id] += elem;
}

static void qbench(size_t num_threads,size_t num_elems) {
  size_t *thread_sum = new size_t [num_threads], total_sum = 0;
  memset(thread_sum,0,num_threads * sizeof *thread_sum);
  pfn_run_threaded(num_threads,num_elems,sum_elements,
                   (void *) thread_sum);
  for (size_t idx = 0; idx < num_threads; idx++) {
    std::cout << "thread_sum[" << idx << "]=" << thread_sum[idx]
              << std::endl;
    total_sum += thread_sum[idx];
  }
  std::cout << "total_sum=" << total_sum << std::endl;
  assert(total_sum == (num_elems * (num_elems - 1))/2);
  delete[] thread_sum;
}

int main(int argc, char *argv[])
{
  size_t num_threads, num_elements;
  long readlong1, readlong2;

  if (argc != 3 || sscanf(argv[1], "%ld", &readlong1) != 1 || readlong1 < 0 ||
      sscanf(argv[2], "%ld", &readlong2) != 1 || readlong2 < 0)
  {
    std::cerr << "Usage: " << argv[0] << " <num_threads> <elements>"
              << std::endl;
    return EXIT_FAILURE;
  }
  num_threads = (size_t) readlong1;
  num_elements = (size_t) readlong2;
  std::cout << num_threads << "\t" << num_elements << "\t"
            << RACE_PREVENTION << std::endl;
  qbench(num_threads,num_elements);
  return EXIT_SUCCESS;
}
