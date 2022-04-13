count = 1000000000

all: enumber.o enumber_np.o

enumber.o: enumber.cpp
	mpic++ enumber.cpp -o enumber.o

enumber_np.o: enumber_np.cpp
	g++ enumber_np.cpp -o enumber_np.o

PHONY: run
run: enumber.o
	mpiexec -n 4 ./enumber.o $(count)

PHONY: run_np
run_np: enumber_np.o
	./enumber_np.o $(count)

PHONY: compare
compare: run run_np

PHONY: clean
clean:
	rm enumber.o enumber_np.o
