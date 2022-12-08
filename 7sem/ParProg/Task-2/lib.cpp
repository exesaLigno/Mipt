#include <math.h>
#include <omp.h>
#include <time.h>
#include <cstring>

unsigned long int clk()
{
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    long int time_ms = time.tv_sec * 1000000 + time.tv_nsec / 1000;
    return time_ms;
}

class Solver
{
private:
    long double* XBand = nullptr;
    long double XStart = 0;
    long double XStop = 0;

    long double* YBand = nullptr;
    long double YStart = 0;
    long double YStop = 0;

    long int N = 0;
    long double H = 0;

    long double* Psi0 = nullptr;
    long double* Psi2 = nullptr;

    long double (*F) (long double x, long double y) = nullptr;
    long double FDerivative(long double x, long double y)
    {
        auto numeric = (F(x, y + H/2.0) - F(x, y - H/2.0)) / H;
        auto calculated = 3.0 * powl(y, 2.0) - 1.0;
        //printf("numeric = %Lg, calculated = %Lg\n", numeric, calculated);
        return calculated;
    }

    bool RunParallel = false;

    long double Precision = 1e-5;
    int IterationLimit = 50;

    long double CurrentError = -1;
    int CurrentIteration = -1;

    long double NumerovApproximation(long int k)
    {
        long double nm = (YBand[k + 1] + YBand[k - 1] - 2.0 * YBand[k]) / (H * H);
        long double addition = (1.0 / 12.0) * (F(XBand[k-1], YBand[k-1]) + F(XBand[k+1], YBand[k+1]));
        //return 6.0 * (nm - addition) / 5.0;
        return nm;
    }

    void CalculatePsi()
    {
        #pragma omp parallel sections num_threads(2) if(RunParallel)
        {
            #pragma omp section
            {
                Psi0[0] = 0.0; Psi0[1] = 0.0;
                for (int k = 1; k < N - 1; k++)
                {
                    long double deriv = FDerivative(XBand[k], YBand[k]);
                    long double diff = F(XBand[k], YBand[k]) - NumerovApproximation(k);
                    Psi0[k + 1] = (Psi0[k] * deriv + diff) * H * H +
                        2.0 * Psi0[k] - Psi0[k - 1];
                }
            }

            #pragma omp section
            {
                Psi2[0] = 0.0; Psi2[1] = H;
                for (int k = 1; k < N - 1; k++)
                {
                    long double deriv = FDerivative(XBand[k], YBand[k]);
                    long double diff = F(XBand[k], YBand[k]) - NumerovApproximation(k);
                    Psi2[k + 1] = (Psi2[k] * deriv + diff) * H * H +
                        2.0 * Psi2[k] - Psi2[k - 1];
                }
            }
        }
    }

    void Iteration()
    {
        CalculatePsi();
        long double coeff = -Psi0[N - 1] / Psi2[N - 1];

        //printf("coeff = %Lg (%Lg, %Lg)\n", coeff, Psi0[N - 1], Psi2[N - 1]);

        #pragma omp parallel for if(RunParallel)
        for (int k = 1; k < N - 1; k++)
            YBand[k] += Psi0[k] + coeff * Psi2[k];
    }

    long double CheckPrecision()
    {
        long double max_error = 0;

        #pragma omp parallel for reduction(max : max_error) if(RunParallel)
        for (int k = 1; k < N - 1; k++)
            max_error = max_error > std::abs(F(XBand[k], YBand[k]) - NumerovApproximation(k)) ?
                        max_error : std::abs(F(XBand[k], YBand[k]) - NumerovApproximation(k));

        return max_error;
    }

public: 
    Solver(long int n, long double (*f) (long double, long double), double x_start = 0, double x_stop = 1, bool parallel = false) : 
        N(n), F(f), XStart(x_start), XStop(x_stop), RunParallel(parallel)
    {
        H = /*(XStop - XStart)*/ 1.0 / ((long double) N);
        XBand = new long double[N] { };
        for (int k = 0; k < N; k++)
            XBand[k] = XStart + (XStop - XStart) * k / ((long double) (N - 1));
        YBand = new long double[N] { };
        Psi0 = new long double[N] { };
        Psi2 = new long double[N] { };
    }

    Solver(const Solver&) = delete;

    ~Solver()
    {
        if (XBand != nullptr) delete[] XBand;
        if (YBand != nullptr) delete[] YBand;
        if (Psi0 != nullptr) delete[] Psi0;
        if (Psi2 != nullptr) delete[] Psi2;
        XBand = nullptr;
        YBand = nullptr;
        Psi0 = nullptr;
        Psi2 = nullptr;
    }

    void SetBounds(long double y_start, long double y_stop)
    {
        YStart = y_start;
        YStop = y_stop;

        #pragma omp parallel for if(RunParallel)
        for (int k = 0; k < N; k++)
            YBand[k] = YStart + (YStop - YStart) * k / ((long double) (N - 1)); // Generating line from startpoint to endpoint
    }

    unsigned long Calculate()
    {
        auto start_time = clk();

        for (CurrentIteration = 1; CurrentIteration <= IterationLimit; CurrentIteration++)
        {
            Iteration();
            CurrentError = CheckPrecision();
            //printf("Cur accuracy: %Lg\n", CurrentError);
            if (CurrentError > 0 and CurrentError < Precision) break;
        }

        return clk() - start_time;
    }

    long double GetPrecision() { return CurrentError; }
    int GetIterations() { return CurrentIteration; }
    long double* GetResult() { return YBand; }
    long int GetPointsCount() { return N; }

    void PrintSolution()
    {
        for (int k = 0; k < N; k++)
            printf("%.4Lf ", YBand[k]);
    }

    void ExportSolution(const char* filename_prefix)
    {
        char filename[strlen(filename_prefix) + 100] = { };
        sprintf(filename, "%s_output_%s.txt", filename_prefix, RunParallel ? "parallel" : "linear");
        FILE* output_file = fopen(filename, "w");
        for (int k = 0; k < N; k++)
            fprintf(output_file, "%.4Lf ", YBand[k]);
        fclose(output_file);
    }
};
