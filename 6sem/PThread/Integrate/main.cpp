#include <fstream>
#include <cmath>
#include "integral.hpp"

const double START = -2.999;
const double STOP = 0;

double function(double parameter) { return cos(1 / (3 + parameter)); }
double quad(double parameter) { return parameter * parameter; }

int main(int argc, char** argv)
{
  auto integral = Integral(function);
  //auto result = integral.Calculate(START, STOP);
  double result2 = integral.Calculate(START, STOP);
  printf("integral = %lg\n", result2);
  return 0;
}


/*
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

  double start = START, stop = STOP, dx = 0.0001;
  printf("Result: %lg\n", integrate(function, start, stop, dx, accuracy));
  return 0;
}


double function(double parameter) { return cos(1 / (3 + parameter)); }

double quad(double param) { return param * param; }

double integrate(double f(double), double start, double stop, double initial_dx, double accuracy)
{
  double integral = 0;

  double dx = initial_dx;

  for (double x = start; x < stop; x += dx)
  {
    double current_accuracy = abs(f(x) * dx - f(x + dx) / f(x));
    if (current_accuracy > accuracy)
    {
      dx /= 2;
      printf("dx increased\n");
    }

    if (current_accuracy * 10 < accuracy)
    {
      dx *= 2;
      printf("dx decreaced\n");
    }

    integral += (f(x) + f(x + dx)) / 2 * dx;
  }

  return integral;
}
*/
