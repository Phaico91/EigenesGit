#ifndef VIRTUAL_QUEUE_HPP
#define VIRTUAL_QUEUE_HPP
#include <cstdlib>
#include <cassert>
#include <mutex>

class VirtualQueue {
  private:
    size_t current, last;
    std::mutex queue_mutex;
  public:
    VirtualQueue(size_t number_of_elements) {
      current = 0;
      assert(number_of_elements > 0);
      last = number_of_elements - 1;
    }
    ~VirtualQueue(void) {}
    size_t next_element(void) {
      size_t element;
      queue_mutex.lock(); /* to prevent data race */
      element = current++;
      queue_mutex.unlock();
      return element;
    }
    size_t last_element(void) const {
      /* needed for terminating next_element loop */
      return last;
    }
};
#endif
