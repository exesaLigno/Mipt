#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "task.cpp"

double** calculate(double x_lim, double h, double t_lim, double tau, double (*phi) (double x), double (*psi) (double t), double (*f) (double x, double t));
void printMatrix(double x_lim, double h, double t_lim, double tau, double** U, bool table);
void exportData(double x_lim, double h, double t_lim, double tau, double** U);

int main(int argc, char** argv)
{
  clock_t start_time = clock();
  double** U = calculate(X_LIM, DX, T_LIM, DT, phi, psi, f);
  clock_t stop_time = clock();
  printf("\x1b[1;33mNon-parallel run completed in %ld ms\x1b[0m\nWriting to file started\n", stop_time - start_time);
  if (EXPORT_TO_FILE) exportData(X_LIM, DX, T_LIM, DT, U);
  return 0;
}


double** calculate(double x_lim, double h, double t_lim, double tau, double (*phi) (double x), double (*psi) (double t), double (*f) (double x, double t))
{
  int t_size = (int) (t_lim / tau), x_size = (int) (x_lim / h);

  double** U = new double*[t_size];
  for (int t = 0; t < t_size; t++)
  {
    U[t] = new double[x_size];
    U[t][0] = psi(t * tau);
  }

  for (int x = 0; x < x_size; x++)
    U[0][x] = phi(x * h);

  for (int t = 1; t < t_size; t++)
  {
    for (int x = 1; x < x_size; x++)
    {
      U[t][x] = (2 * f(h * (x + 1/2), tau * (t + 1/2)) -
                (1/tau - 1/h) * (U[t][x-1] + U[t-1][x]) +
                (1/tau + 1/h) * U[t - 1][x - 1]) / (1/tau + 1/h);
    }
  }

  return U;
}

void printMatrix(double x_lim, double h, double t_lim, double tau, double** U, bool table)
{
  int t_size = (int) (t_lim / tau), x_size = (int) (x_lim / h);

  printf("       ");   // Printing only 6 symbols of number
  if (table) printf(" ┃ ");

  for (int x = 0; x < x_size; x++)
  {
    printf("%-6lg ", x * h);
    if (x != x_size - 1 and table)
      printf("╎ ");
  }

  if (table)
  {
    printf("\n");

    printf("━━━━━━━╋━");
    for (int x = 0; x < x_size; x++)
    {
      printf("━━━━━━");
      if (x != x_size - 1)
        printf("━┿━");
    }
  }

  printf("\n");
  for (int t = 0; t < t_size; t++)
  {
    printf("%-6lg ", t * tau);
    if (table) printf("┃ ");
    for (int x = 0; x < x_size; x++)
    {
      printf("%-6lg ", U[t][x]);
      if (x != x_size - 1 and table)
        printf("╎ ");
    }

    if (table)
    {
      printf("\n");

      if (t != t_size - 1)
      {
        printf("╌╌╌╌╌╌╌╂╌");
        for (int x = 0; x < x_size; x++)
        {
          printf("╌╌╌╌╌╌", U[t][x]);
          if (x != x_size - 1)
            printf("╌┼╌");
        }
      }
    }

    printf("\n");
  }
}

void exportData(double x_lim, double h, double t_lim, double tau, double** U)
{
  int t_size = (int) (t_lim / tau), x_size = (int) (x_lim / h);

  FILE* file = fopen("result_np.txt", "w");

  for (int x = 0; x < x_size; x++)
  {
    for (int t = 0; t < t_size; t++)
      fprintf(file, "%lg ", U[t][x]);
    fprintf(file, "\n");
  }

  fclose(file);
}
