#include <iostream>
#include <thread>

static void print_msg(const char *ptr)
{
  std::cout << ptr << std::endl;
}

int main(void)
{
  const char *msg1 = "Thread 1";
  const char *msg2 = "Thread 2";

  /* Create independent threads each of which will
     execute same function with different args. */
  std::thread t1 (print_msg, msg1);
  std::thread t2 (print_msg, msg2);
  /* Wait until threads are complete before main continues.
     Unless we wait we have the risk of executing an exit
     which will terminate the process and all threads
     before the threads have completed. */

  t1.join();
  t2.join();
  std::cout << "Thread 1 finished" << std::endl;
  std::cout << "Thread 2 finished" << std::endl;
  return EXIT_SUCCESS;
}
