#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <omp.h>
#include <cstring>
#include <math.h>
#include "matrix.cpp"


long clk();
void test_correctness(int matrix_size);
void test_multipliers(int matrix_size);
void measure_times(int max_matrix_size);



int main(int argc, char** argv)
{
    srand(time(nullptr));
    omp_set_nested(true);

    if (argc > 1 and !strcmp(argv[1], "show"))
        test_correctness(5);

    else if (argc > 1 and !strcmp(argv[1], "times"))
        test_multipliers(1000);

    else if (argc > 1 and !strcmp(argv[1], "measure"))
        measure_times(1500);

    else 
        printf("Using: %s [show, measure]\n", argv[0]);

    return 0;
}


long clk()
{
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    long time_ms = time.tv_sec * 1000 + time.tv_nsec / 1000000;
    return time_ms;
}


void test_correctness(int matrix_size)
{
    if (matrix_size <= 10)
    {
        Matrix* M1 = new Matrix(matrix_size);
        Matrix* M2 = new Matrix(matrix_size);
        M1 -> FillRandom();
        M2 -> FillRandom();
        auto reference = M1 -> LinearMultiply(M2);

        printf("Matrix M1\n");
        M1 -> Print();
        printf("\nMatrix M2\n");
        M2 -> Print();
        printf("\nM1 * M2 = \n");
        reference -> Print();
    }
    else
        printf("Don't showing large matrix's\n");
}


void test_multipliers(int matrix_size)
{
    long start_time = 0;
    Matrix* M1 = new Matrix(matrix_size);
    Matrix* M2 = new Matrix(matrix_size);
    M1 -> FillRandom();
    M2 -> FillRandom();

    start_time = clk();
    auto reference = M1 -> LinearMultiply(M2);
    long linear_time = clk() - start_time;

    start_time = clk();
    auto linear_fast_multiplied = M1 -> FastMultiplyLinear(M2);
    long linear_fast_time = clk() - start_time;

    start_time = clk();
    auto parallel_multiplied = M1 -> ParallelMultiply(M2);
    long parallel_time = clk() - start_time;

    start_time = clk();
    auto fast_multiplied = M1 -> FastMultiply(M2);
    long fast_time = clk() - start_time;

    start_time = clk();
    auto simd_multiplied = M1 -> SimdMultiply(M2);
    long simd_time = clk() - start_time;

    printf("%s Linear:     %10ld ms\n"
           "%s LinearFast: %10ld ms\n"
           "%s Parallel:   %10ld ms\n"
           "%s Fast:       %10ld ms\n"
           "%s Simd:       %10ld ms\n", 
        "✅", linear_time,
        *reference == *linear_fast_multiplied ? "✅": "❌", linear_fast_time, 
        *reference == *parallel_multiplied ? "✅": "❌", parallel_time, 
        *reference == *fast_multiplied ? "✅": "❌", fast_time, 
        *reference == *simd_multiplied ? "✅": "❌", simd_time);
}



void measure_times(int max_matrix_size)
{
    int matrix_size = 10;
    double multiplier = 12;
    long start_time = 0;

    FILE* times = fopen("times.csv", "w");
    fprintf(times, "MATRIX_SIZE; LINEAR_TIME; FAST_LINEAR_TIME; PARALLEL_TIME; FAST_TIME; SIMD_TIME\n");

    while (matrix_size < max_matrix_size)
    {
        Matrix* M1 = new Matrix(matrix_size);
        Matrix* M2 = new Matrix(matrix_size);
        M1 -> FillRandom();
        M2 -> FillRandom();

        start_time = clk();
        auto reference = M1 -> LinearMultiply(M2);
        long linear_time = clk() - start_time;

        start_time = clk();
        auto linear_fast_multiplied = M1 -> FastMultiplyLinear(M2);
        long linear_fast_time = clk() - start_time;

        start_time = clk();
        auto parallel_multiplied = M1 -> ParallelMultiply(M2);
        long parallel_time = clk() - start_time;

        start_time = clk();
        auto fast_multiplied = M1 -> FastMultiply(M2);
        long fast_time = clk() - start_time;

        start_time = clk();
        auto simd_multiplied = M1 -> SimdMultiply(M2);
        long simd_time = clk() - start_time;

        fprintf(times, "%11d; %11ld; %16ld; %13ld; %9ld; %9ld\n", 
            matrix_size * matrix_size, linear_time, linear_fast_time, parallel_time, fast_time, simd_time);

        int old_matrix_size = matrix_size;
        matrix_size = matrix_size * multiplier;
        multiplier = sqrt(multiplier);

        if (matrix_size - old_matrix_size < 3)
            matrix_size = old_matrix_size + 3;
    }
    
    fclose(times);
}
