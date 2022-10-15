GRID_SIZE = 7000

all: task.o task_mpi.o

task.o: task.cpp
	g++ task.cpp -o task.o -std=c++2a

task_mpi.o: task_MPI.cpp
	mpic++ task_MPI.cpp -o task_mpi.o -std=c++2a

.PHONY: run
run: run_linear run_mpi

.PHONY: run_linear
run_linear: task.o
	./task.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: run_mpi
run_mpi: task_mpi.o
	mpiexec -quiet -n 4 ./task_mpi.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: clean
clean:
	rm *.o result_*.txt
