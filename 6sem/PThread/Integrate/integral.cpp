#include "integral.hpp"



Integral::Integral(double (*function) (double), double accuracy, int threads_count)
{
  this -> f = function;
  this -> accuracy = accuracy;
  this -> threads_count = threads_count;
}

Integral::~Integral()
{}



double Integral::SimpleRiman(double start_point, double stop_point, double dx)
{
  double current_point = start_point;
  double result = 0;

  while (current_point < stop_point)
  {
    result += (f(current_point) + f(current_point + dx)) / 2 * dx;
    current_point += dx;
  }

  return result;
}


// Parallel integration
double Integral::Calculate(double start_point, double stop_point)
{
  // Initializing pthread fields
  pthread_mutex_init(&(this -> mutex), NULL);
  pthread_t threads[this -> threads_count] = {};
  this -> thread_number = 0;

  // Initializing task pool to balance process between threads
  this -> tasks_count = pow(this -> threads_count, 2);
  this -> task_number = 0;

  // Initializing integration borders
  this -> start_point = start_point;
  this -> stop_point = stop_point;

  double result = 0; // Storing result here

  // Starting threads
  // Here we using a trick - calling function-wrapper and forwarding
  // this as parameter
  for (int i = 0; i < this -> threads_count; i++)
    pthread_create(threads + i, NULL, this -> _threadRoutine, this);

  // Collecting results and joining threads
  for (int i = 0; i < this -> threads_count; i++)
  {
    double* thread_result = nullptr;
    pthread_join(threads[i], (void**) &thread_result);
    result += *thread_result;
    delete thread_result;
  }

  // Destroying pthread fields
  pthread_mutex_destroy(&(this -> mutex));

  return result;
}

// Function-wrapper, calling ThreadRoutine for this, forwarded as
// parameter for wrapper
void* Integral::_threadRoutine(void* this_pointer)
{
  double* thread_result = new double;
  *thread_result = ((Integral*) this_pointer) -> ThreadRoutine();
  return (void*) thread_result;
}

double Integral::ThreadRoutine()
{
  int thread_number = -1;
  pthread_mutex_lock(&(this -> mutex));
    thread_number = (this -> thread_number)++;
  pthread_mutex_unlock(&(this -> mutex));
  printf("Thread %d started\n", thread_number);

  double thread_result = 0;
  int solved_tasks = 0;
  clock_t start_time = clock();

  while (true)
  {
    int task_number = 0;
    bool task_taken = true;
    pthread_mutex_lock(&(this -> mutex));
      if (this -> task_number < this -> tasks_count)
        task_number = (this -> task_number)++;
      else
        task_taken = false;
    pthread_mutex_unlock(&(this -> mutex));

    if (not task_taken)
      break;

    double interval_size = (this -> stop_point - this -> start_point) / this -> tasks_count;
    double local_start_point = this -> start_point + interval_size * task_number;
    double local_stop_point = this -> start_point + interval_size * (task_number + 1);

    printf("Thread %d solving task %d [%lg; %lg)\n", thread_number, task_number, local_start_point, local_stop_point);

    double local_integral = this -> SimpleRiman(local_start_point, local_stop_point);
    thread_result += local_integral;
    solved_tasks++;

    printf("Thread %d solved task %d. Result is %lg\n", thread_number, task_number, local_integral);
  }

  clock_t stop_time = clock();

  printf("Thread %d solved %d tasks in %ld ms\n", thread_number, solved_tasks, stop_time - start_time);

  return thread_result;
}
