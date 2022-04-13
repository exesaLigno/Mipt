#include <cstdio>
#include <mpi.h>

const int DEFAULT_TAG = 1;

int main(int argc, char** argv)
{
  int data = 1337;

  MPI_Init(&argc, &argv);
  int comm_size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Status status;

  if (rank == 0)
  {
    MPI_Send(&data, 1, MPI_INT, (rank + 1) % comm_size, DEFAULT_TAG, MPI_COMM_WORLD);
    printf("%d: Sended data = %d to %d process\n", rank, data, (rank + 1) % comm_size);
    MPI_Recv(&data, 1, MPI_INT, comm_size - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
    printf("%d: RESULT: Received data = %d from %d process\n", rank, data, comm_size - 1);
  }

  else
  {
    MPI_Recv(&data, 1, MPI_INT, (rank - 1), DEFAULT_TAG, MPI_COMM_WORLD, &status);
    printf("%d: Received data = %d from %d process and changed to %d\n", rank, data, rank - 1, data + 1);
    data++;
    MPI_Send(&data, 1, MPI_INT, (rank + 1) % comm_size, DEFAULT_TAG, MPI_COMM_WORLD);
    printf("%d: Sended changed data = %d to process %d\n", rank, data, (rank + 1) % comm_size);
  }

  MPI_Finalize();
  return 0;
}
