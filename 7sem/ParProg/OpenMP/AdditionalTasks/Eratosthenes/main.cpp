#include <cstdio>
#include <cstdlib>
#include "simple.cpp"
#include "clocking.cpp"

int main(int argc, char** argv)
{
    if (argc < 2)
        return 1;

    int N = atoi(argv[1]);

    long linear_time_start = clk();
    int* simple_numbers_linear = EratosthenesThinOut(N, false);
    long linear_time = clk() - linear_time_start;

    if (N <= 200)
    {
        printf("Linear:   ");
        ShowResults(N, simple_numbers_linear);
    }

    long parallel_time_start = clk();
    int* simple_numbers_parallel = EratosthenesThinOut(N, true);
    long parallel_time = clk() - parallel_time_start;

    if (N <= 200)
    {
        printf("Parallel: ");
        ShowResults(N, simple_numbers_parallel);
    }

    printf("Linear and parallel results are %s\n", 
        SameResults(N, simple_numbers_linear, simple_numbers_parallel) ? 
        "\x1b[1;32msame\x1b[0m" : "\x1b[1;31mnot same\x1b[0m");

    printf("\nTimes for N = %d\n  Linear:   %ld mks\n  Parallel: %ld mks\n", N, linear_time, parallel_time);

    return 0;
}
