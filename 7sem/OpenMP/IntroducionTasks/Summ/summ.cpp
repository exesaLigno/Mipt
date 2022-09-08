#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define DEBUG false

int main(int argc, char** argv)
{
  if (argc <= 1) return 1;
  int N = atoi(argv[1]);

  double final_sum = 0;

  #pragma omp parallel
  {
    int threads_count = omp_get_num_threads();
    int thread_num = omp_get_thread_num();

    int start_number = 1 + thread_num * N / threads_count;
    int stop_number = (thread_num + 1) * N / threads_count;

    double sum = 0;

    for (int number = start_number; number <= stop_number; number++)
    {
      sum += 1 / (double) number;
    }

    if (DEBUG) printf("My thread_num is %d of %d runners, i'm calculating summ on [%d, %d], calculated sum = %lg\n", thread_num, threads_count, start_number, stop_number, sum);

    #pragma omp atomic
      final_sum += sum;
  }

  printf("Final summ is %lg\n", final_sum);

  return 0;
}
