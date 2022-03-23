all: summ.o

summ.o: summ.cpp
	mpic++ summ.cpp -o summ.o

PHONY: run
run: summ.o
	mpiexec -n 4 ./summ.o 10000

PHONY: clean
clean:
	rm summ.o
