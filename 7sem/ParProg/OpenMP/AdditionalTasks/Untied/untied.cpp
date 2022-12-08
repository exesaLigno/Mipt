#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int variable = 100;
#pragma omp threadprivate(variable)

FILE* output = fopen("log.txt", "a");

void setVar()
{
    variable = 1337;
}

void sleep()
{
    usleep(rand() % 5000);
}

void printVar()
{
    fprintf(output, "\t%d\n", variable);
}

void unsafe(int iter_num)
{
    fprintf(output, "> Run %d\n", iter_num);
    #pragma omp parallel
    {
        #pragma omp task untied
        {
            setVar();
            sleep();
            printVar();
        }
            
        //printf("hello world\n");
    }
}

int main()
{
    for (int iter = 1; iter <= 10000; iter++)
    {
        unsafe(iter);
    }

    fclose(output);
}
