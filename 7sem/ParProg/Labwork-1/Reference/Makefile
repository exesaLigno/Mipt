GRID_SIZE = 7000
THREADS_COUNT = 8
WORKERS_COUNT = 4

all: ref.o ref_mpi.o ref_omp.o

ref.o: reference.cpp
	g++ reference.cpp -o ref.o -std=c++2a

ref_omp.o: reference_OpenMP.cpp
	g++ reference_OpenMP.cpp -o ref_omp.o -std=c++2a -fopenmp

ref_mpi.o: reference_MPI.cpp
	mpic++ reference_MPI.cpp -o ref_mpi.o -std=c++2a

.PHONY: run
run: run_linear run_omp run_mpi
	echo "Comparing OpenMP variant with linear..."
	../compare.sh result_linear.txt result_omp.txt
	echo "Comparing MPI variant with linear..."
	../compare.sh result_linear.txt result_mpi.txt

.PHONY: run_linear
run_linear: ref.o
	./ref.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: run_omp
run_omp: ref_omp.o
	./ref_omp.o $(GRID_SIZE) $(GRID_SIZE) $(THREADS_COUNT)

.PHONY: run_mpi
run_mpi: ref_mpi.o
	mpiexec -quiet -n $(WORKERS_COUNT) ./ref_mpi.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: clean
clean:
	rm *.o result_*.txt
