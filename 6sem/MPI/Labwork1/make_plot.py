#!/usr/bin/python

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import re
import numpy as np

data = open('result_np.txt', 'r')
task = open('task.cpp', 'r')

task_text = task.read()
t_lim_founded = re.findall(r'T_LIM ?= ?([\d.]+)', task_text)
x_lim_founded = re.findall(r'X_LIM ?= ?([\d.]+)', task_text)
t_lim = float(t_lim_founded[0])
x_lim = float(x_lim_founded[0])

DATA = []
for line in data:
    DATA.append(list(map(float, line.split())))

x_band = np.linspace(0, x_lim, len(DATA))
y_band = np.linspace(0, t_lim, len(DATA[0]))
data_band = np.array(DATA)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x_band, y_band, data_band, cmap='inferno')
plt.savefig('result.png')
