#include <fstream>
#include <cmath>
#include "integral.hpp"

namespace Assignment
{
  double function(double parameter) { return cos(1 / (3 + parameter)); }
  const double start = -2.999;
  const double stop = 0;
};

namespace Quad
{
  double function(double parameter) { return parameter * parameter; }
  const double start = 0;
  const double stop = 10;
};

using namespace Assignment; // Only for quick changing of function and limits

int main(int argc, char** argv)
{
  int threads_count = 0;
  double accuracy = 0;

  if (argc < 3)
  {
    printf("Usage: %s [threads_count] [accuracy]\n", argv[0]);
    return 1;
  }
  else
  {
    threads_count = atoi(argv[1]);
    accuracy = atof(argv[2]);
  }

  auto integral = Integral(function, accuracy, threads_count);
  integral.thread_summary = true;
  double result = integral.Calculate(start, stop);
  printf("Integral for assigned function is \x1b[1;32m%lg\x1b[0m\n", result);
  return 0;
}
