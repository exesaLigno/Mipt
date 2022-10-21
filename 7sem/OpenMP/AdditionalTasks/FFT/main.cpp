#include <cstdio>
#include <cstdlib>
#include "lib/FFT.cpp"
#include "lib/clocking.cpp"

void show_results();


int main(int argc, char** argv)
{
    show_results();

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
