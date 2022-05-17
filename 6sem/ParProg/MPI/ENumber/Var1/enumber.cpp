#include <cstdio>
#include <mpi.h>
#include <ctime>

const int CALCULATED_SUM = 1;
const int DEBUG_FLAG = false;
#define DEBUG if(DEBUG_FLAG)

int main(int argc, char** argv)
{
  int x = 1;

  if (argc <= 1)
    return 1;

  int count = atoi(argv[1]);

  MPI_Init(&argc, &argv);
  int comm_size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  clock_t start_time, stop_time;

  if (rank == 0)
    start_time = clock();

  int start_number = rank * count / comm_size;
  int stop_number = (rank + 1) * count / comm_size - 1;

  DEBUG printf("%d: Calculating part from %d to %d\n", rank, start_number, stop_number);

  long double current_addition = 1;
  long double partitial_sum = current_addition;

  for (int counter = start_number + 1; counter <= stop_number; counter++)
  {
    current_addition = current_addition * x / (float) counter;
    partitial_sum += current_addition;
  }

  DEBUG printf("%d: Calculated my part: %.30Lf\n", rank, partitial_sum);

  if (rank != comm_size - 1)
  {
    long double next_part_sum = 0;
    long double next_part_multiplier = current_addition * x / (float) stop_number;
    DEBUG printf("%d: Waiting next part from %d\n", rank, rank+1);
    MPI_Recv(&next_part_sum, 1, MPI_LONG_DOUBLE, rank + 1, CALCULATED_SUM, MPI_COMM_WORLD, &status);
    partitial_sum += next_part_sum * next_part_multiplier;
    DEBUG printf("%d: Added next part: %.30Lf with multiplier %.30Lf; result: %.30Lf\n", rank, next_part_sum, next_part_multiplier, partitial_sum);
  }

  if (rank != 0)
  {
    DEBUG printf("%d: Sending result %.30Lf to %d\n", rank, partitial_sum, rank - 1);
    MPI_Send(&partitial_sum, 1, MPI_LONG_DOUBLE, rank - 1, CALCULATED_SUM, MPI_COMM_WORLD);
  }

  if (rank == 0)
  {
    stop_time = clock();
    printf("Calculated number e^%d = %.30Lf\n", x, partitial_sum);
    printf("Executed in %d milliseconds\n", stop_time - start_time);
  }

  MPI_Finalize();
}
