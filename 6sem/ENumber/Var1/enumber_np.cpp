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

  printf("Calculated number e^%d = %llg in %d milliseconds\n", x, result, stop_time - start_time);
}
