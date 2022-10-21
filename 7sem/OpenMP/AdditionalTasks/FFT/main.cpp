#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "lib/FFT.cpp"
#include "lib/clocking.cpp"

void show_results();
void read_signal(double* signal, long signal_length);
void write_spectre(cpl* spectre, long signal_length);
void compare_times(long amount);
void calculate(long amount);

int main(int argc, char** argv)
{
    if (argc > 1 and !strcmp(argv[1], "show"))
        show_results();
    
    else if (argc > 2 and !strcmp(argv[1], "compare"))
        compare_times(atol(argv[2]));

    else if (argc > 2 and !strcmp(argv[1], "calc"))
        calculate(atol(argv[2]));

    return 0;
}


void show_results()
{
    long sig_size = 8;
    double* signal = new double[sig_size]{1.2, 1.6, 1.9, 1.3, 0.9, 0.1, -0.4, -1.1};
    cpl* spectrum = new cpl[sig_size];

    DFT(signal, sig_size, spectrum, false);
    printf("DFT: ");
    for (int i = 0; i < 8; i++)
        spectrum[i].print("; ");
    printf("\n");

    FFT(signal, sig_size, spectrum, true);
    printf("FFT: ");
    for (int i = 0; i < 8; i++)
        spectrum[i].print("; ");
    printf("\n");
}

void read_signal(double* signal, long signal_length)
{
    char filename[200] = {};
    sprintf(filename, "signals/%ld/%ld_signal.txt", signal_length, signal_length);
    FILE* signal_file = fopen(filename, "r");
    int n = 0;
    double readed = 0;
    while (fscanf(signal_file, "%lf", &(signal[n])) == 1)
        n++;
    fclose(signal_file);
}

void write_spectre(cpl* spectre, long signal_length)
{
    char filename[200] = {};
    sprintf(filename, "signals/%ld/%ld_spectre.txt", signal_length, signal_length);
    FILE* spectre_file = fopen(filename, "w");
    for (int i = 0; i < signal_length; i++)
        fprintf(spectre_file, "%.4lf ", spectre[i].abs());
    fclose(spectre_file);
}

void compare_times(long amount)
{
    double* signal = new double[amount];
    cpl* spectre = new cpl[amount];

    read_signal(signal, amount);

    auto start_time = clk();
    FFT(signal, amount, spectre, false);
    auto linear_time = clk() - start_time;

    start_time = clk();
    FFT(signal, amount, spectre, true);
    auto parallel_time = clk() - start_time;

    printf("For %ld points: \n Linear = %ld mks \n Parallel = %ld mks\n", amount, linear_time, parallel_time);
}

void calculate(long amount)
{
    double* signal = new double[amount];
    cpl* spectre = new cpl[amount];

    read_signal(signal, amount);

    FFT(signal, amount, spectre);

    write_spectre(spectre, amount);
}
