#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define SUCCESSIVELY_THREAD_START true

int main(int argc, char** argv)
{
  int data = 1337;
  int running_thread_num = 0;

  #pragma omp parallel
  {
    int thread_num = omp_get_thread_num();
    if (SUCCESSIVELY_THREAD_START) while (running_thread_num != thread_num);

    #pragma omp critical
    {
      printf("I am thread %d of %d. Changing value from %d to %d.\n", thread_num, omp_get_num_threads(), data, data+1);
      data++;
      running_thread_num++;
    }
  }

  return 0;
}
