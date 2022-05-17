#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>

const double T_LIM = 10;
const double DT = 0.005;

const double X_LIM = 10;
const double DX = 0.005;

double f(double x, double t) { return pow(x * t, 0.3); }
double phi(double x) { return sin(x) * x; }
double psi(double t) { return sin(t) * t; }

double** calculate(double x_lim, double h, double t_lim, double tau, double (*phi) (double x), double (*psi) (double t), double (*f) (double x, double t));
void exportData(double x_lim, double h, double t_lim, double tau, double** U);

int main(int argc, char** argv)
{
  double** U = calculate(X_LIM, DX, T_LIM, DT, phi, psi, f);
  exportData(X_LIM, DX, T_LIM, DT, U);
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

void exportData(double x_lim, double h, double t_lim, double tau, double** U)
{
  int t_size = (int) (t_lim / tau), x_size = (int) (x_lim / h);

  //FILE* file = fopen("result.txt", "w");

  for (int x = 0; x < x_size; x++)
  {
    for (int t = 0; t < t_size; t++)
      printf("%lg ", U[t][x]);
    printf("\n");
  }

  //fclose(file);
}
