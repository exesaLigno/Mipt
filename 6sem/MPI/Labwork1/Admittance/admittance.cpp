#include <mpi.h>
#include <ctime>

const int DEFAULT_TAG = 1;

long int clk()
{
  timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  long int time_ns = time.tv_sec * 1000000000 + time.tv_nsec;
  return time_ns;
}

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

  int repeatition_count = 100000;

  if (rank == 0)
  {
    start_time = clk();
    for (int i = 0; i < repeatition_count; i++)
    {
      MPI_Send(&temp, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD);
      MPI_Recv(&temp, 1, MPI_INT, size - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
    }

    stop_time = clk();
    printf("Message is passing between ranks about \x1b[1m%lg ns\x1b[0m\n", (stop_time - start_time) / (double) (repeatition_count * size));
  }

  else
  {
    for (int i = 0; i < repeatition_count; i++)
    {
      MPI_Recv(&temp, 1, MPI_INT, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
      MPI_Send(&temp, 1, MPI_INT, (rank + 1)%size, DEFAULT_TAG, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();

  return 0;
}
