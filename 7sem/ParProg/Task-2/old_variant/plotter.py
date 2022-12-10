#!/usr/bin/python

import sys
import os
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

for root, dir, files in os.walk("."):
    for filename in files:
        if filename.endswith(".txt"):
            with open(filename) as file:
                #x_band = np.linspace(-10, 10, len(y_band))
                x_str, y_str = file.read().split('\n')
                x_band = list(map(float, x_str.split(' ')[:-1]))                
                y_band = list(map(float, y_str.split(' ')[:-1]))
                

                plt.figure(figsize=[14,8])
                plt.grid()
                plt.title(filename)
                plt.plot(x_band, y_band)
                plt.savefig(f'{filename[:-4]}.png')
