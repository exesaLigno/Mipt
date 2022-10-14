#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <mpi.h>
#include "../clocking.cpp"
const int DEFAULT_HEIGHT = 1000;
const int DEFAULT_WIDTH = 1000;

const int DATA_FORWARDING = 1;

double** createGrid(int height, int width, double (*filler)(int, int));
void deleteGrid(double** grid, int height, int width);
void exportData(const char* filename, double** grid, int width, int height);
double defaultFiller(int line, int column);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank = 0, comm_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    int height = DEFAULT_HEIGHT, width = DEFAULT_WIDTH;

    if (argc == 3)
    {
        height = atoi(argv[1]);
        width = atoi(argv[2]);
    }

    else if (rank == 0)
        printf("Using default grid size %d×%d.\n"
               "If you want to specify size, use \x1b[1m%s [height] [width]\x1b[0m\n",
               DEFAULT_HEIGHT, DEFAULT_WIDTH, argv[0]);

    auto grid = createGrid(height, width, defaultFiller);
    
    long start_time = clk();

    int start_number = rank * height / comm_size;
    int stop_number = (rank + 1) * height / comm_size - 1;

    for (int line = start_number; line <= stop_number; line++)
    {
        for (int column = 0; column < width; column++)
            grid[line][column] = sin(2 * grid[line][column]);
    }

    if (rank != 0)
    {
        for (int sending_line = start_number; sending_line <= stop_number; sending_line++)
            MPI_Send(grid[sending_line], width, MPI_DOUBLE, 0, DATA_FORWARDING, MPI_COMM_WORLD);
    }

    else
    {
        for (int sender_rank = 1; sender_rank < comm_size; sender_rank++)
        {
            int sender_start_number = sender_rank * height / comm_size;
            int sender_stop_number = (sender_rank + 1) * height / comm_size - 1;

            for (int receiving_line = sender_start_number; receiving_line <= sender_stop_number; receiving_line++)
                MPI_Recv(grid[receiving_line], width, MPI_DOUBLE, sender_rank, DATA_FORWARDING, MPI_COMM_WORLD, &status);
        }
    }

    long duration = clk() - start_time;

    if (rank == 0)
    {
        printf("   MPI: executed in %ld ms for grid %d×%d\n", 
                duration, height, width);

        exportData("result_mpi.txt", grid, height, width);
    }

    deleteGrid(grid, height, width);

    MPI_Finalize();

    return 0;
}


double defaultFiller(int line, int column)
{
    return 10 * line + column;
}


double** createGrid(int height, int width, double (*filler)(int, int))
{
    double** grid = new double*[height];

    for (int line = 0; line < height; line++)
    {
        grid[line] = new double[width];

        for (int column = 0; column < height; column++)
            grid[line][column] = filler(line, column);
    }

    return grid;
}


void deleteGrid(double** grid, int height, int width)
{
    for (int line = 0; line < height; line++)
        delete[] grid[line];

    delete[] grid;
}


void exportData(const char* filename, double** grid, int height, int width)
{
    FILE* output_file = fopen(filename,"w");

    for(int line = 0; line < height; line++)
    {
        for (int column = 0; column < width; column++)
            fprintf(output_file, "%f ", grid[line][column]);

        fprintf(output_file, "\n");
    }

    fclose(output_file);
}
