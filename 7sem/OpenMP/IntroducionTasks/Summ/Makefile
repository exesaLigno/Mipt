all: summ.o

summ.o: summ.cpp
	mpic++ summ.cpp -o summ.o -fopenmp

PHONY: clean
clean:
	rm summ.o
