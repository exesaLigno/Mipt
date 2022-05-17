#include <cmath>

const double T_LIM = 10;
const double DT = 0.01;

const double X_LIM = 10;
const double DX = 0.01;

double f(double x, double t) { return 1; }
double phi(double x) { return sin(x) * x; }
double psi(double t) { return 0; }

const bool EXPORT_TO_FILE = true;
