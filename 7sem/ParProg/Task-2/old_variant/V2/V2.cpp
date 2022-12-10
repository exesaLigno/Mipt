#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "../lib.cpp"

long N = 10;
bool Silent = false;
const long double XStart = 0;
const long double XStop = 1;
const long double YStart = 1;
const long double YStop = 1.5;

long double f(long double x, long double y) { return expl(-y); }

int main(int argc, char** argv)
{
    if (argc >= 2) N = atoi(argv[1]);
    if (argc >= 3) Silent = !strcmp(argv[2], "-s");

    char prefix[50] = { };
    sprintf(prefix, "V1_%.1Lf", YStop);

    auto linear_solver = Solver(N, f, XStart, XStop, false);
    linear_solver.SetBounds(YStart, YStop);
    auto linear_time = linear_solver.Calculate();
    linear_solver.ExportSolution(prefix);

    auto parallel_solver = Solver(N, f, XStart, XStop, true);
    parallel_solver.SetBounds(YStart, YStop);
    auto parallel_time = parallel_solver.Calculate();
    parallel_solver.ExportSolution(prefix);

    if (not Silent)
    {
        printf("For %ld points on [%Lg, %Lg]\n", N, XStart, XStop);
        printf("Accuracy in linear:   %Lg (%d iterations)\n", linear_solver.GetPrecision(), linear_solver.GetIterations());
        printf("Accuracy in parallel: %Lg (%d iterations)\n", parallel_solver.GetPrecision(), parallel_solver.GetIterations());
        printf("Linear time:   %ld mks\n", linear_time);
        printf("Parallel time: %ld mks\n", parallel_time);
    }

    else
        printf("│ %15ld │ %15ld │ %15ld │\n", N, linear_time, parallel_time);

    return 0;
}
