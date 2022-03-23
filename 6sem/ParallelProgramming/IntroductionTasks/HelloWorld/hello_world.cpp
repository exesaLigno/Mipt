#include <cstdio>
#include <mpi.h>

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int comm_size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("Hello world! My rank is %d of %d runners\n", rank, comm_size);
  MPI_Finalize();
  return 0;
}
