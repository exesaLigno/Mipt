#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
  int x = 1;

  if (argc <= 1)
    return 1;

  int count = atoi(argv[1]);

  auto start_time = clock();

  long double current_addition = 1;
  long double result = current_addition;

  for (int counter = 1; counter < count; counter++)
  {
    current_addition = current_addition * x / (float) counter;
    result += current_addition;
  }

  auto stop_time = clock();

  printf("Calculated number e^%d = %.30Lf\n", x, result);
  printf("Executed in %d milliseconds\n", stop_time - start_time);
}
