#pragma once
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>

long int clk();

class Integral
{
// This fields connected to integral object
private:
  double (*f) (double) = nullptr;
  double accuracy = 0;
  int threads_count = 0;

// This fields connected to calculation only
private:
  pthread_mutex_t mutex;
  int tasks_count = 0;
  int task_number = 0;
  int thread_number = 0;
  double start_point = 0;
  double stop_point = 0;

  static void* _threadRoutine(void* this_pointer);  // pthread_create can't start thread with non-static method, so it is the solution)
  double ThreadRoutine();

public:
  bool verbose_process = false;
  bool thread_summary = false;

  Integral(double (*function) (double), double accuracy = 0.0001, int threads_count = 8);
  ~Integral();

  double SimpleRiman(double start_point, double stop_point, double dx = 0.000000001);
  double Recursive(double start_point, double stop_point, double accuracy);
  double Calculate(double start_point, double stop_point);
};
