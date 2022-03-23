#include <cstdio>
#include <mpi.h>
#include <cstring>
#include <math.h>

const int CALCULATED_SUM = 1;
const bool DEBUG = false;

double calculateSumInOneProcess(int start, int stop)
{
  double sum = 0;
  for (int k = start; k <= stop; k++)
    sum += 1 / (double) k;
  return sum;
}

int main(int argc, char** argv)
{
  if (argc <= 1) return 1;

  MPI_Init(&argc, &argv);

  int N = atoi(argv[1]);
  int comm_size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int start_number = 1 + rank * N / comm_size;
  int stop_number = (rank + 1) * N / comm_size;

  double sum = 0;

  for (int number = start_number; number <= stop_number; number++)
  {
    sum += 1 / (double) number;
  }

  if (DEBUG) printf("My rank is %d of %d runners, i'm calculating summ on [%d, %d], calculated sum = %lg\n", rank, comm_size, start_number, stop_number, sum);

  if (rank != 0)
    MPI_Send(&sum, 1, MPI_DOUBLE, 0, CALCULATED_SUM, MPI_COMM_WORLD);

  else
  {
    double final_sum = sum;
    double received_sum = 0;
    MPI_Status status;
    for (int sender_rank = 1; sender_rank < comm_size; sender_rank++)
    {
      MPI_Recv(&received_sum, 1, MPI_DOUBLE, sender_rank, CALCULATED_SUM, MPI_COMM_WORLD, &status);
      if (DEBUG) printf("Received sum = %lg from process with rank %d\n", received_sum, sender_rank);
      final_sum += received_sum;
    }

    printf("Sum of 1/n for n in [%d, %d] is \x1b[1;32m%lg\x1b[0m", 1, N, final_sum);
    if (DEBUG)
    {
      double reference_summ = calculateSumInOneProcess(1, N);
      printf(", Reference summ, calculated in one process is \x1b[1;32m%lg\x1b[0m", reference_summ);
      if (abs(reference_summ - final_sum)/final_sum < 0.00000001)
        printf(" \x1b[1;32mCorrect\x1b[0m");
      else
        printf(" \x1b[1;31mIncorrect\x1b[0m");
    }
    printf("\n");
  }

  MPI_Finalize();
}
