GRID_SIZE = 7000

all: task.o task_omp_v1.o task_omp_v2.o

task.o: task.cpp
	g++ task.cpp -o task.o -std=c++2a

task_omp_v1.o: task_OpenMP_v1.cpp
	g++ task_OpenMP_v1.cpp -o task_omp_v1.o -std=c++2a -fopenmp

task_omp_v2.o: task_OpenMP_v2.cpp
	g++ task_OpenMP_v2.cpp -o task_omp_v2.o -std=c++2a -fopenmp

.PHONY: run
run: run_linear run_omp_v1 run_omp_v2

.PHONY: run_linear
run_linear: task.o
	./task.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: run_omp_v1
run_omp_v1: task_omp_v1.o
	./task_omp_v1.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: run_omp_v2
run_omp_v2: task_omp_v2.o
	./task_omp_v2.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: clean
clean:
	rm *.o result_*.txt
