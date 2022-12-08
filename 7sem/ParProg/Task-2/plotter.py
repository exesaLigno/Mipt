#!/usr/bin/python

import sys
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

if len(sys.argv) < 2: print('specify filename')
file = open(sys.argv[1])
y_band = list(map(float, file.read().split(' ')[:-1]))
x_band = np.linspace(-10, 10, len(y_band))

plt.figure(figsize=[14,8])
plt.grid()
plt.title(sys.argv[1])
plt.plot(x_band, y_band)
plt.savefig(f'{sys.argv[1][:-4]}.png')
