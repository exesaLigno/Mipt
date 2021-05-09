#!/usr/bin/env python3

import os

print("Hi, I will clean your directory after using to save storage, but actually I don't work at all now :(")

path = os.curdir
for el in os.walk(path):
	print(el)
