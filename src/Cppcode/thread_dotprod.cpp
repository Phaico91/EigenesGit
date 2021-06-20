/*
  This example program illustrates the use of mutex variables
  in a threads program. The main data is made available to all threads
  through a pointer to a Vectors object.
  Each thread works on a different part of the data. The main thread waits
  for all the threads to complete their computations, and then it prints
  the resulting sum.
  The original implementation was from Vijay Sonnad, IBM using pthreads.
  The recoding in C++ was done by Stefan Kurtz
*/

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <thread>
#include <mutex>
#include <vector>

/*
  The following class contains the necessary information
  to allow the function "dotprod" to access its input data and
  compute its output.
*/

class Vectors {
  public:
    double *a_vec, *b_vec;
    size_t _veclen;
    Vectors(size_t veclen) {
      a_vec = new double [veclen];
      b_vec = new double [veclen];
      srand48(366292341);
      for (size_t idx = 0; idx < veclen; idx++) {
        a_vec[idx] = drand48();
        b_vec[idx] = drand48();
      }
      _veclen = veclen;
    }
    ~Vectors(void) {
      delete[] a_vec;
      delete[] b_vec;
    }
};

/*
  The function dotprod is activated when the thread is created.
  As before, all input to this function is obtained from a class
  Vectors and all output from this function is written into
  the member variables of this structure. The benefit of this approach is
  apparent for the
  multi-threaded program: when a thread is created we pass a single
  argument to the activated function.
*/

static void dotprod(double *sum,std::mutex *mutex_sum,
                    const Vectors *vectors, size_t threadnum,
                    size_t part_len)
{
  const size_t start = threadnum * part_len,
               end = start + part_len;
  double threadsum = 0.0;

  for (size_t idx = start; idx<end; idx++)
    threadsum += (vectors->a_vec[idx] * vectors->b_vec[idx]);
  mutex_sum->lock();
  *sum += threadsum;
  printf("thread %lu [%7lu,%7lu]: threadsum=%.2f sum=%.2f\n",
         threadnum,start,end,threadsum,*sum);
  mutex_sum->unlock();
}

/*
  The main program creates threads which do all the work and then
  print out result upon completion. Before creating the threads,
  the input data is created. Since all threads update the same sum-variable,
  we need a mutex for mutual exclusion. The main thread needs to wait for
  all threads to complete.
*/

int main(void) {
  constexpr const size_t numthreads = 4, part_len = 100000;
  Vectors vectors(numthreads * part_len);
  double sum = 0.0;
  std::mutex mutex_sum;
  std::vector<std::thread> threads{};

  for (size_t idx=0; idx<numthreads; idx++) {
    threads.push_back(std::thread(dotprod,&sum,&mutex_sum,
                                  &vectors,idx,part_len));
  }
  /* compact loop for accessing elements of container */
  for (auto &th : threads) th.join();
  printf("sum = %.2f \n", sum);
  return EXIT_SUCCESS;
}
