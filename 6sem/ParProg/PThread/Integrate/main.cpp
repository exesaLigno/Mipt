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

  long int start_time, stop_time;
  start_time = clk();
  double result = integral.Calculate(start, stop);
  stop_time = clk();

  printf("Integral is \x1b[1;32m%lg\x1b[0m (%ld ms in parallel method)\n", result, stop_time - start_time);

  start_time = clk();
  // result = integral.SimpleRiman(start, stop, accuracy);
  result = integral.Recursive(start, stop, accuracy);
  stop_time = clk();

  printf("Integral is \x1b[1;32m%lg\x1b[0m (%ld ms in linear method)\n", result, stop_time - start_time);
  return 0;
}
