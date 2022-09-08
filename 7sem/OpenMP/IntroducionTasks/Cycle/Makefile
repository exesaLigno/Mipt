all: cycle.o

cycle.o: cycle.cpp
	mpic++ cycle.cpp -o cycle.o -fopenmp

PHONY: clean
clean:
	rm cycle.o
