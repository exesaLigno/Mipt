#include <cmath>

const double T_LIM = 10;
const double TAU = 0.01;

const double X_LIM = 10;
const double H = 0.01;

double f(double x, double t) { return 1; }
double phi(double x) { return sin(2 * 3.14 * x); }
double psi(double t) { return sqrt(t); }

const bool EXPORT_TO_FILE = true;
