#!/usr/bin/python

import numpy as np
import sys
import matplotlib.pyplot as plt

def CheckAmountCorrectness(amount):
    t = 1
    while t < amount:
        t *= 2
    return t == amount

def GenerateComplexSignal(amount):
    f = lambda x: 10 * (np.sin(2 * np.pi * x) + np.sin(15 * np.pi * x) + np.sin(100000 * np.pi * x))

    x_band = np.linspace(0, 1, amount)
    f_band = f(x_band)

    return f_band, np.fft.fft(f_band)

def DrawFFTPlot(filename):
    file = open(filename, 'r')
    values = list(map(float, file.read().split()))
    file.close()

    plt.figure(figsize=[12, 5])
    plt.title(f'{filename}')
    plt.plot(values)
    plt.savefig(f'{filename[:-4]}_plot.png')
    

if __name__ == "__main__":
    if len(sys.argv) == 3 and sys.argv[1] == 'generate':
        amount = int(sys.argv[2])
        if not CheckAmountCorrectness(amount):
            print(f'{amount} is incorrect value for amount, use powers of 2')
            sys.exit()

        f_band, spectre = GenerateComplexSignal(int(amount))

        file = open(f'signals/{amount}/{amount}_signal.txt', 'w')
        for f in f_band:
            file.write(f'{f:.4f} ')
        file.close()

        file = open(f'signals/{amount}/{amount}_reference_spectre.txt', 'w')
        for s in spectre:
            file.write(f'{np.abs(s):.4f} ')
        file.close()

    elif len(sys.argv) == 3 and sys.argv[1] == 'plot':
        DrawFFTPlot(sys.argv[2])

