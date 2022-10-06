#include "sudoku.cpp"
#include <cstring>

int main(int argc, char** argv)
{
    bool run_parallel = false;
    bool run_linear = true;

    char* filename = nullptr;
    if (argc > 1)
        filename = argv[1];
    else
    {
        printf("No task provided\n");
        return 1;
    }

    if (argc > 2 and !strcmp(argv[2], "parallel"))
    {
        run_parallel = true;
        run_linear = false;
    }

    else if (argc > 2 and !strcmp(argv[2], "linear"))
    {
        run_parallel = false;
        run_linear = true;
    }

    else if (argc > 2 and !strcmp(argv[2], "both"))
    {
        run_parallel = true;
        run_linear = true;
    }

    else 
        printf("No mode provided, running linear\n");

    int** field = nullptr;
    int field_size = 0;
    ReadTask(filename, &field, &field_size);

    printf("Task \"%s\"\n", filename);
    ShowField(field, field_size);

    int** solution = nullptr;
    bool single_solution = false;

    long linear_time = 0, parallel_time = 0;

    if (run_parallel)
        parallel_time = SolveSudoku(field, field_size, &solution, &single_solution, true);

    if (run_linear)
        linear_time = SolveSudoku(field, field_size, &solution, &single_solution, false);
    
    if (solution != nullptr) 
        ShowField(solution, field_size);
    
    if (solution == nullptr)
        printf("Incorrect sudoku, no solutions found!\n");
    else if (not single_solution)
        printf("Incorrect sudoku, multiple solutions found\n");

    if (run_linear and run_parallel)
    {
        FILE* times_file = fopen("times.txt", "a");
        fprintf(times_file, "%10d; %20ld; %20ld\n", EmptyCellsCount(field, field_size), linear_time, parallel_time);
        printf("Executed in %d mks in parallel, %d mks in linear\n", parallel_time, linear_time);
    }
    else
        printf("Executed in %d mks (%s run)\n", time, run_parallel ? "parallel" : "linear");

    return 0;
}
