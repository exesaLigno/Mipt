#include <pthread.h>
#include <fstream>

const int DEBUG_FLAG = false;
#define DEBUG if(DEBUG_FLAG)

int THREADS_COUNT = 4;
int THREAD_NUM = 0;

pthread_mutex_t MUTEX;

void* calcSummPart(void* range_size_pointer);

int main(int argc, char** argv)
{
  if (argc <= 1)
    return 1;

  int range_size = atoi(argv[1]);

  pthread_t threads[THREADS_COUNT] = {};
  pthread_mutex_init(&MUTEX, NULL);

  for (int i = 0; i < THREADS_COUNT; i++)
    pthread_create(threads + i, NULL, calcSummPart, &range_size);

  double summ = 0;

  for (int i = 0; i < THREADS_COUNT; i++)
  {
    void* partitial_summ_addr = nullptr;
    pthread_join(threads[i], &partitial_summ_addr);
    double partitial_summ = *(double*)partitial_summ_addr;
    summ += partitial_summ;
    DEBUG printf("Added %lg to summ, summ now is %lg\n", partitial_summ, summ);
    //delete partitial_summ_addr;
  }

  printf("\x1b[1;32mCalculated summ for %d addendum is %lg\x1b[0m\n", range_size, summ);

  pthread_mutex_destroy(&MUTEX);

  return 0;
}


void* calcSummPart(void* range_size_pointer)
{
  pthread_mutex_lock(&MUTEX);
    int rank = THREAD_NUM++;
  pthread_mutex_unlock(&MUTEX);

  int range_size = *(int*)range_size_pointer;

  int start_number = 1 + rank * range_size / THREADS_COUNT;
  int stop_number = (rank + 1) * range_size / THREADS_COUNT;

  double* summ = new double();

  for (int number = start_number; number <= stop_number; number++)
    *summ += 1 / (double) number;

  DEBUG printf("I am #%d of %d threads, i'm calculating summ on [%d, %d], calculated sum = %lg\n", rank, THREADS_COUNT, start_number, stop_number, *summ);

  return (void*)summ;
}
