#include <cstdio>
#include <mpi.h>
#include <ctime>


int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int comm_size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  clock_t start_time, stop_time;

  if (rank == 0)
    start_time = clock();

  if (rank == 0)
  {
    stop_time = clock();
    printf("Executed in %d milliseconds\n", stop_time - start_time);
  }

  MPI_Finalize();
}
