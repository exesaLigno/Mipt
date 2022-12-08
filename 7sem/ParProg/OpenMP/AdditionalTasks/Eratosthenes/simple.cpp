#pragma once
#include <omp.h>

void ShowResults(int maximum_number, int* thinned_out_numbers)
{
    for (int i = 0; i <= maximum_number; i++)
        if (thinned_out_numbers[i] != 0) printf("%d ", thinned_out_numbers[i]);
    printf("\n");
}

bool SameResults(int maximum_number, int* thinned_out_numbers_1, int* thinned_out_numbers_2)
{
    for (int i = 0; i <= maximum_number; i++)
        if (thinned_out_numbers_1[i] != thinned_out_numbers_2[i]) return false;
    return true;
}

int* EratosthenesThinOut(int maximum_number, bool run_parallel = true)
{
    omp_set_nested(true);

    int* numbers = new int[maximum_number + 1] { };
    
    for (int i = 2; i <= maximum_number; i++)
        numbers[i] = i;

    //#pragma omp parallel for schedule(guided, 4) if(run_parallel)
    for (int analyzing_number = 2; analyzing_number <= maximum_number; analyzing_number++)
    {
        if (numbers[analyzing_number] == 0)
            continue;
        
        else
        {
            #pragma omp parallel for schedule(static, 4) if(run_parallel)
            for (int erasing_number = analyzing_number * 2; erasing_number <= maximum_number; erasing_number += analyzing_number)
                numbers[erasing_number] = 0;
        }
    }

    return numbers;
}
