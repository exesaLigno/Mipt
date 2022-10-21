#pragma once
#define USE_COMPLEX
#include "complex.cpp"
#include <cmath>
#include <algorithm>

/// @brief Simple discrete Fourier transform
/// @param signal Discretized signal
/// @param length Signal length
/// @param spectrum Empty allocated memory for spectrum (length of spectrum must be equal to signal length)
void DFT(double* signal, long length, cpl* spectrum)
{
    for (int k = 0; k < length; k++)
    {
        for (int n = 0; n < length; n++)
        {
            spectrum[k] += cpl(signal[n]) * (cpl(M_E)^(cpl(0, 
                -2 * M_PI * k * n / length)));
        }

        spectrum[k] = spectrum[k];
    }
}

/// @brief Fast Fourier transform
/// @param signal Discretized signal
/// @param length Signal length
/// @param spectrum Empty allocated memory for spectrum (length of spectrum must be equal to signal length)
void FFT(double* signal, long length, cpl* spectrum)
{
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
        cpl W = cpl(1, 0);

        for (int m = 0; m < Mmax; m++)
        {
            W = cpl(W.re() - (W.re() * Wp.re() + W.im() * Wp.im()),
                    W.im() + W.re() * Wp.im() - W.im() * Wp.re());

            for (int i = m; i < length; i += Mmax * 2)
            {
                cpl tmp = W * spectrum[i + Mmax];
                spectrum[i + Mmax] = spectrum[i] - tmp;
                spectrum[i] = spectrum[i] + tmp;
            }
        }
    }

    for (int i = 0, j = length - 1; i < j; i++, j--)
        std::swap(spectrum[i], spectrum[j]);
}
