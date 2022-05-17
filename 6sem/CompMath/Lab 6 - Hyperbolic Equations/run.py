#!/usr/bin/python

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import re
import numpy as np
import os

compilation = os.popen('g++ -o solve.out solve.cpp -pthread')
if (compilation.close() == None):
    print('Compilation success')
    data = os.popen('./solve.out', 'r')
    task = open('solve.cpp', 'r')

    task_text = task.read()
    task.close()
    t_lim_founded = re.findall(r'T_LIM ?= ?([\d.]+)', task_text)
    x_lim_founded = re.findall(r'X_LIM ?= ?([\d.]+)', task_text)
    t_lim = float(t_lim_founded[0])
    x_lim = float(x_lim_founded[0])

    DATA = []
    for line in data:
        DATA.append(list(map(float, line.split())))
    data.close()

    x_band = np.linspace(0, x_lim, len(DATA))
    t_band = np.linspace(0, t_lim, len(DATA[0]))
    y, x = np.meshgrid(t_band, x_band)
    data_band = np.array(DATA)

    print('Result calculated')

    image_name = 'result.png'
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(x, y, data_band, cmap='inferno')
    plt.savefig(image_name)

    print(f'Plot exported to {image_name}')
else:
    print('Compilation error')
    for line in compilation:
        print(line)
