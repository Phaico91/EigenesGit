#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>

struct Sharedvars
{
  std::mutex mutex1;
  size_t counter;
};

static void increment(Sharedvars *sv)
{
  sv->mutex1.lock();
  sv->counter++;
  sv->mutex1.unlock();
}

int main(void)
{
  const size_t numthreads = 10;
  std::vector<std::thread> thread_tab{};
  Sharedvars sv;

  sv.counter = 0;

  /* Create independent threads each of which
     will execute increment */

  for (size_t t = 0; t < numthreads; t++)
    thread_tab.push_back(std::thread(increment,&sv));

  /* loop uses compact notation for vec-iteration */
  for (auto &th : thread_tab) 
    th.join();
  std::cout << "counter=" << sv.counter << std::endl;
  return EXIT_SUCCESS;
}
