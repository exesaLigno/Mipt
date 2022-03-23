all: cycle.o

cycle.o: cycle.cpp
	mpic++ cycle.cpp -o cycle.o

PHONY: run
run: cycle.o
	mpiexec -n 4 ./cycle.o

PHONY: clean
clean:
	rm cycle.o
