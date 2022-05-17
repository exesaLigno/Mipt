#include <mpi.h>
#include <ctime>

long int clk()
{
  timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  long int time_ms = time.tv_sec * 1000 + time.tv_nsec / 1000000;
  return time_ms;
}

enum TAGS { DEFAULT_TAG, PREVIOUS_VALUE_TAG, DATA_WRITED_TAG, TASK_SOLVED_TAG, DATA_COLLECTING_TAG = 4 };

#define COLLECT

#include "task.cpp"

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int rank = 0, comm_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  int temp = 0;

  clock_t start_time = 0, stop_time = 0;

  if (rank == 0)
    start_time = clk();

  int t_size = (int) (T_LIM / DT), x_size = (int) (X_LIM / DX);

  int start_number = rank * x_size / comm_size;
  int stop_number = (rank + 1) * x_size / comm_size - 1;
  int current_x_size = stop_number - start_number + 1;

  double** current_U = new double*[t_size]; // Creating matrix of solution
  for (int t = 0; t < t_size; t++)
    current_U[t] = new double[rank == 0 ? x_size : current_x_size + 1];  // Matrix in ranks greater than 0 will be 'crossed' with previos one in the beginning

  for (int x = 0; x < current_x_size + (rank == 0 ? 0 : 1); x++)  // Setting up values for initial time moment
    current_U[0][x] = phi((x + start_number - (rank == 0 ? 0 : 1)) * DX);

  for (int t = 1; t < t_size; t++)
  {
    if (rank == 0)  // Setting initial position in different times in rank 0
      current_U[t][0] = psi(t * DT);
    else  // Getting value from previous process and setting it into matrix as initial for this rank
      MPI_Recv(&(current_U[t][0]), 1, MPI_DOUBLE, rank-1, PREVIOUS_VALUE_TAG, MPI_COMM_WORLD, &status);

    for (int x = 1; x < current_x_size + (rank == 0 ? 0 : 1); x++)  // Calculating all values in row for this t
    {
      current_U[t][x] = (2 * f(DX * (x + start_number - (rank == 0 ? 0 : 1) + 1/2), DT * (t + 1/2)) -
                        (1/DT - 1/DX) * (current_U[t][x-1] + current_U[t-1][x]) +
                        (1/DT + 1/DX) * current_U[t - 1][x - 1]) / (1/DT + 1/DX);
    }

    if (rank != comm_size - 1)  // Sending last value to the next rank to make it start to work
      MPI_Send(&(current_U[t][current_x_size - (rank == 0 ? 1 : 0)]), 1, MPI_DOUBLE, rank+1, PREVIOUS_VALUE_TAG, MPI_COMM_WORLD);
  }

#ifndef COLLECT

  if (rank == comm_size - 1)
    MPI_Send(&temp, 1, MPI_INT, 0, TASK_SOLVED_TAG, MPI_COMM_WORLD);

  if (rank == 0)
  {
    MPI_Recv(&temp, 1, MPI_INT, comm_size-1, TASK_SOLVED_TAG, MPI_COMM_WORLD, &status);
    stop_time = clk();
    printf("\x1b[1;33mParallel run completed in %ld ms\x1b[0m\n", stop_time - start_time);
  }

  if (EXPORT_TO_FILE)
  {
    printf("Writing to file started\n")
    FILE* file = nullptr;

    if (rank != 0)
      MPI_Recv(&temp, 1, MPI_INT, rank-1, DATA_WRITED_TAG, MPI_COMM_WORLD, &status);

    file = fopen("result.txt", rank == 0 ? "w" : "a");

    for (int x = (rank == 0 ? 0 : 1); x < current_x_size + (rank == 0 ? 0 : 1); x++)
    {
      for (int t = 0; t < t_size; t++)
        fprintf(file, "%lg ", current_U[t][x]);
      fprintf(file, "\n");
    }

    fclose(file);

    if (rank != comm_size - 1)
      MPI_Send(&temp, 1, MPI_INT, rank+1, DATA_WRITED_TAG, MPI_COMM_WORLD);
  }

#else

  if (rank == 0)
  {
    for (int sender_rank = 1; sender_rank < comm_size; sender_rank++)
    {
      int sender_start_num = sender_rank * x_size / comm_size;
      int sender_stop_num = (sender_rank + 1) * x_size / comm_size - 1;
      int sender_current_x_size = sender_stop_num - sender_start_num + 1;

      for (int t = 0; t < t_size; t++)
        MPI_Recv(current_U[t] + sender_start_num, sender_current_x_size, MPI_DOUBLE, sender_rank, DATA_COLLECTING_TAG, MPI_COMM_WORLD, &status);
    }

    stop_time = clk();
    printf("\x1b[1;33mParallel run completed in %ld ms\x1b[0m\n", stop_time - start_time);

#ifdef MEASURE_TIMES
    FILE* fp = fopen("times_n.txt", "a");
    if (comm_size == 1)
      fprintf(fp, "%d, %d", x_size * t_size, stop_time - start_time);
    else
      fprintf(fp, ", %d", stop_time - start_time);
    if (comm_size == 4)
      fprintf(fp, "\n");
    fclose(fp);
#endif

    if (EXPORT_TO_FILE)
    {
      printf("Writing to file started\n");
      FILE* file = fopen("result.txt", "w");

      for (int x = 0; x < x_size; x++)
      {
        for (int t = 0; t < t_size; t++)
          fprintf(file, "%lg ", current_U[t][x]);
        fprintf(file, "\n");
      }

      fclose(file);
    }
  }

  else
  {
    for (int t = 0; t < t_size; t++)
      MPI_Send(current_U[t] + 1, current_x_size, MPI_DOUBLE, 0, DATA_COLLECTING_TAG, MPI_COMM_WORLD);
  }

#endif

  MPI_Finalize();

  return 0;
}
