#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define DEBUG true

int main(int argc, char **argv)
{
    if (argc <= 1)
        return 1;
    int N = atoi(argv[1]);

    double summ = 0;

    #pragma omp parallel for reduction(+:summ)
    for (int number = 1; number <= N; number++)
        summ += 1 / (double)number;

    printf("Final summ is %.50lg\n", summ);

    return 0;
}
