#pragma once
#define USE_COMPLEX
#include "complex.cpp"
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <omp.h>

/// @brief Simple discrete Fourier transform
/// @param signal Discretized signal
/// @param length Signal length
/// @param spectrum Empty allocated memory for spectrum (length of spectrum must be equal to signal length)
void DFT(double* signal, long length, cpl* spectrum, bool run_parallel = false)
{
    #pragma omp parallel for schedule(static, 4) if(run_parallel)
    for (int k = 0; k < length; k++)
    {
        #pragma omp parallel for schedule(static, 4) if(run_parallel)
        for (int n = 0; n < length; n++)
        {
            spectrum[k] += cpl(signal[n]) * (cpl(M_E)^(cpl(0, 
                -2 * M_PI * k * n / length)));
        }

        spectrum[k] = spectrum[k];
    }
}

/// @brief Optimized fast Fourier transform
/// @param signal Discretized signal
/// @param length Signal length
/// @param spectrum Empty allocated memory for spectrum (length of spectrum must be equal to signal length)
void FFT(double* signal, long length, cpl* spectrum, bool run_parallel = false)
{
    #pragma omp parallel for schedule(static, 4) if(run_parallel)
    for (int i = 0; i < length; i++)
        spectrum[i] = signal[i];

    for (int i = 1, j = 1; i < length * 2; i += 2)
    {
        if (j > i)
            std::swap(spectrum[i / 2], spectrum[j / 2]);

        int m;
        for (m = length; m >= 2 and j > m; m /= 2) 
            j -= m;

        j += m;
    }

    for (int Mmax = 1; Mmax < length; Mmax *= 2)
    {
        cpl Wp = cpl(sin(-M_PI / (Mmax * 2)) * sin(-M_PI / (Mmax * 2)) * 2, -sin(-2 * M_PI / (Mmax * 2)));

        int threads_count = Mmax < 8 ? Mmax : 8;

        #pragma omp parallel if(run_parallel) num_threads(threads_count)
        {
            threads_count = omp_get_num_threads();
            int rank = omp_get_thread_num();
            int start_m = rank * Mmax / threads_count;
            int stop_m = (rank + 1) * Mmax / threads_count - 1;

            //printf("Mmax = %d, thread %d calculating on [%d %d]\n", Mmax, rank, start_m, stop_m);

            cpl W = cpl(1, 0);

            for (int i = 0; i < start_m; i++)
                W = cpl(W.re() - (W.re() * Wp.re() + W.im() * Wp.im()),
                        W.im() + W.re() * Wp.im() - W.im() * Wp.re());

            for (int m = start_m; m <= stop_m; m++)
            {
                W = cpl(W.re() - (W.re() * Wp.re() + W.im() * Wp.im()),
                        W.im() + W.re() * Wp.im() - W.im() * Wp.re());

                #pragma omp parallel for schedule(static, 4) if(run_parallel)
                for (int i = m; i < length; i += Mmax * 2)
                {
                    cpl tmp = W * spectrum[i + Mmax];
                    spectrum[i + Mmax] = spectrum[i] - tmp;
                    spectrum[i] = spectrum[i] + tmp;
                }
            }
        }
    }

    #pragma omp parallel for schedule(static, 4) if(run_parallel)
    for (int i = 0; i < length / 2; i++)
    {
        int j = length - 1 - i;
        std::swap(spectrum[i], spectrum[j]);
    }
}
