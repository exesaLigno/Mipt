#include <omp.h>
#include <cstdio>

int x = 8;
#pragma omp threadprivate(x)

int main(int argc, char** argv)
{
    omp_set_num_threads(4);

    x = 150;

    // In this team only main thread will have new value of x, other threads 
    // will have default value (8)
    #pragma omp parallel
    {
        printf("Value for x \x1b[1;31mwithout\x1b[0m copyin() is %d in "
               "thread %d\n", x, omp_get_thread_num());
    }

    // In this team x value of main thread will be copied to other threads
    #pragma omp parallel copyin(x)
    {
        printf("Value for x \x1b[1;32mwith\x1b[0m copyin() is %d in thread "
               "%d\n", x, omp_get_thread_num());
    }

    // The value of x is copied already, so in this team we can see that all
    // threads has same value
    #pragma omp parallel
    {
        printf("Value for x \x1b[1;31mwithout\x1b[0m copyin() \x1b[1;31magain"
               "\x1b[0m is %d in thread %d\n", x, omp_get_thread_num());
    }

    x = 1000000;

    // When we setting new value for x in main thread, other threads doesn't 
    // know about it
    #pragma omp parallel
    {
        printf("\x1b[1;33mNew\x1b[0m value for x \x1b[1;31mwithout\x1b[0m "
               "copyin() \x1b[1;31magain\x1b[0m is %d in thread %d\n", x, 
               omp_get_thread_num());
    }

    return 0;
}
