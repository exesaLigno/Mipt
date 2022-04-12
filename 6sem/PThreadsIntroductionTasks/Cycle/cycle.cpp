#include <pthread.h>
#include <fstream>

int THREADS_COUNT = 4;

pthread_mutex_t MUTEX;

int global_variable = 0;

void* threadFunction(void* _);

int main(int argc, char** argv)
{
  if (argc <= 1)
    return 1;

  THREADS_COUNT = atoi(argv[1]);
  pthread_t threads[THREADS_COUNT] = {};
  pthread_mutex_init(&MUTEX, NULL);

  for (int i = 0; i < THREADS_COUNT; i++)
    pthread_create(threads + i, NULL, threadFunction, NULL);

  for (int i = 0; i < THREADS_COUNT; i++)
    pthread_join(threads[i], NULL);

  pthread_mutex_destroy(&MUTEX);

  printf("Final value is %d\n", global_variable);

  return 0;
}


void* threadFunction(void* _)
{
  pthread_mutex_lock(&MUTEX);
    int old_var = global_variable++;
    printf("Hi, I am thread #%d and I'm changed variable from %d to %d\n", old_var, old_var, global_variable);
  pthread_mutex_unlock(&MUTEX);
  return nullptr;
}
