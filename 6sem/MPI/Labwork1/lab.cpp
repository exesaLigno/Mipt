#include <mpi.h>

const int PREVIOUS_VALUE_TAG = 1;
const int DATA_WRITED_TAG = 2;

#include "task.cpp"

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int rank = 0, comm_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  int t_size = (int) (T_LIM / TAU), x_size = (int) (X_LIM / H);

  int start_number = rank * x_size / comm_size;
  int stop_number = (rank + 1) * x_size / comm_size - 1;
  int current_x_size = stop_number - start_number + 1;

  double** current_U = new double*[t_size]; // Creating matrix of solution
  for (int t = 0; t < t_size; t++)
    current_U[t] = new double[current_x_size + (rank == 0 ? 0 : 1)];  // Matrix in ranks greater than 0 will be 'crossed' with previos one in the beginning

  for (int x = 0; x < current_x_size + (rank == 0 ? 0 : 1); x++)  // Setting up values for initial time moment
    current_U[0][x] = phi((x + start_number - (rank == 0 ? 0 : 1)) * H);

  for (int t = 1; t < t_size; t++)
  {
    if (rank == 0)  // Setting initial position in different times in rank 0
      current_U[t][0] = psi(t * TAU);
    else  // Getting value from previous process and setting it into matrix as initial for this rank
      MPI_Recv(&(current_U[t][0]), 1, MPI_DOUBLE, rank-1, PREVIOUS_VALUE_TAG, MPI_COMM_WORLD, &status);

    for (int x = 1; x < current_x_size + (rank == 0 ? 0 : 1); x++)  // Calculating all values in row for this t
    {
      current_U[t][x] = (2 * f(H * (x + 1/2), TAU * (t + 1/2)) -
                        (1/TAU - 1/H) * (current_U[t][x-1] + current_U[t-1][x]) +
                        (1/TAU + 1/H) * current_U[t - 1][x - 1]) / (1/TAU + 1/H);
    }

    if (rank != comm_size - 1)  // Sending last value to the next rank to make it start to work
      MPI_Send(&(current_U[t][current_x_size - (rank == 0 ? 1 : 0)]), 1, MPI_DOUBLE, rank+1, PREVIOUS_VALUE_TAG, MPI_COMM_WORLD);
  }

  int writed = 0;
  FILE* file = nullptr;

  if (rank != 0)
    MPI_Recv(&writed, 1, MPI_INT, rank-1, DATA_WRITED_TAG, MPI_COMM_WORLD, &status);

  file = fopen("result.txt", rank == 0 ? "w" : "a");

  for (int x = (rank == 0 ? 0 : 1); x < current_x_size + (rank == 0 ? 0 : 1); x++)
  {
    for (int t = 0; t < t_size; t++)
      fprintf(file, "%lg ", current_U[t][x]);
    fprintf(file, "\n");
  }

  fclose(file);

  if (rank != comm_size - 1)
    MPI_Send(&writed, 1, MPI_INT, rank+1, DATA_WRITED_TAG, MPI_COMM_WORLD);

  MPI_Finalize();
}
