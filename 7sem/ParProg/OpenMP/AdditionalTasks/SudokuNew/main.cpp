#include "sudoku.hpp"

int main()
{
    omp_set_nested(true);

    auto solver = SudokuSolver();
    solver.readTask(nullptr);

    printf("Task %s\n", "debug");
    solver.showField();

    auto exec_time = solver.run();

    printf("Solved task %s\n", "debug");
    solver.showField();
    printf("Solved in %ld mks\n", exec_time);

    return 0;
}
