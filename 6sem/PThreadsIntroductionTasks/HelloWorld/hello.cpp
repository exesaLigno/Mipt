#include <pthread.h>
#include <fstream>

int THREADS_COUNT = 0;
int THREAD_NUM = 0;
pthread_mutex_t MUTEX;

void* helloThread(void* temp);

int main(int argc, char** argv)
{
  if (argc <= 1)
    return 1;

  THREADS_COUNT = atoi(argv[1]);
  pthread_t threads[THREADS_COUNT] = {};
  pthread_mutex_init(&MUTEX, NULL);

  for (int i = 0; i < THREADS_COUNT; i++)
    pthread_create(threads + i, NULL, helloThread, NULL);

  for (int i = 0; i < THREADS_COUNT; i++)
    pthread_join(threads[i], NULL);

  pthread_mutex_destroy(&MUTEX);

  return 0;
}


void* helloThread(void* thread_num_pointer)
{
  pthread_mutex_lock(&MUTEX);
    printf("Hello world, I am thread #%d of %d\n", THREAD_NUM, THREADS_COUNT);
    THREAD_NUM++;
  pthread_mutex_unlock(&MUTEX);
  return nullptr;
}
