#include <mpi.h>
#include <ctime>

const int DEFAULT_TAG = 1;

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int rank = 0, size = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  long int start_time = 0;
  long int stop_time = 0;
  int temp = 0;


  if (rank == 0)
  {
      start_time = clock();
      MPI_Send(&temp, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD);
      MPI_Recv(&temp, 1, MPI_INT, size - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
      stop_time = clock();
      printf("Delivery time is %ld\n ", (stop_time - start_time) / size);
  }

  else
  {
      MPI_Recv(&temp, 1, MPI_INT, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
      MPI_Send(&temp, 1, MPI_INT, (rank + 1)%size, DEFAULT_TAG, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
