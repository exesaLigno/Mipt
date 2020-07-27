def getFib(number, x)
	if number <= 1
		return 1
	else
		return getFib(number-1) + getFib(number-2)
	
	
n = input()
res = getFib(n)
print(res)

return 0
