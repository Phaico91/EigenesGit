#include <mutex>
std::mutex my_mutex;
my_muteex.lock();
perform_computation_in_one_thread(); 
/* all other threads are blocked */
my_mutex.unlock();
