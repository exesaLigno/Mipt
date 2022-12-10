#!/usr/bin/python

from random import choice

def generateKey():
	nums = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F']
	num_string = ''
	for _ in range(0, 16):
		num_string += choice(nums)
	return f'0x{num_string}LL'

