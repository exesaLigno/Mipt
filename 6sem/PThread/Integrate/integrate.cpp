#include <pthread.h>
#include <fstream>

/*
class Integrator
{
  double f(double); // Integrating function pointer
  double start = 0;
  double stop = 0;

  Integrator(double f(double), double start, double stop)
  {
    this -> f = f;
    this -> start = start;
    this -> stop = stop;
  }
};
*/

double quad(double parameter);
double integrate(double f(double), double start, double stop, double dx);

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    printf("Usage: ./integrate [threads_count] [accuracy]");
    return 1;
  }

  int threads_count = atoi(argv[1]);
  double accuracy = atof(argv[2]);

  printf("Calculating integral on %d threads with %lg accuracy\n", threads_count, accuracy);

  double start = 1.2, stop = 9.1, dx = 0.001;
  printf("Result: %lg\n", integrate(quad, start, stop, dx));
  return 0;
}


double quad(double parameter)
{
  return parameter * parameter;
}

double integrate(double f(double), double start, double stop, double dx)
{
  double integral = 0;

  for (double x = start; x < stop; x += dx)
    integral += (f(x) + f(x + dx)) / 2 * dx;

  return integral;
}
