GRID_SIZE = 5000

all: ref.o ref_mpi.o ref_omp.o

ref.o: reference.cpp
	g++ reference.cpp -o ref.o -std=c++2a

ref_omp.o: reference_OpenMP.cpp
	g++ reference_OpenMP.cpp -o ref_omp.o -std=c++2a -fopenmp

ref_mpi.o: reference_MPI.cpp
	mpic++ reference_MPI.cpp -o ref_mpi.o -std=c++2a

.PHONY: run
run: run_linear run_omp run_mpi

.PHONY: run_linear
run_linear: ref.o
	./ref.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: run_omp
run_omp: ref_omp.o
	./ref_omp.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: run_mpi
run_mpi: ref_mpi.o
	mpiexec -quiet -n 4 ./ref_mpi.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: clean
clean:
	rm *.o result_*.txt
