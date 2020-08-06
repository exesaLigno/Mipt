def getFib(number, x)
	if number <= 2
		return 1
	else
		return getFib(number-1) + getFib(number-2)
	
	
n = input()
res = getFib(n)
print(res)
