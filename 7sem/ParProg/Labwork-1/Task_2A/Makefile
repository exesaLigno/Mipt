GRID_SIZE = 7000
THREADS_COUNT = 8

all: task.o task_omp_v1.o task_omp_v2.o

task.o: task.cpp
	g++ task.cpp -o task.o -std=c++2a

task_omp_v1.o: task_OpenMP_v1.cpp
	g++ task_OpenMP_v1.cpp -o task_omp_v1.o -std=c++2a -fopenmp

task_omp_v2.o: task_OpenMP_v2.cpp
	g++ task_OpenMP_v2.cpp -o task_omp_v2.o -std=c++2a -fopenmp

.PHONY: run
run: run_linear run_omp_v1 run_omp_v2
	echo "Comparing OpenMP_v1 variant with linear..."
	../compare.sh result_linear.txt result_omp_v1.txt
	echo "Comparing OpenMP_v2 variant with linear..."
	../compare.sh result_linear.txt result_omp_v2.txt

.PHONY: run_linear
run_linear: task.o
	./task.o $(GRID_SIZE) $(GRID_SIZE)

.PHONY: run_omp_v1
run_omp_v1: task_omp_v1.o
	./task_omp_v1.o $(GRID_SIZE) $(GRID_SIZE) $(THREADS_COUNT)

.PHONY: run_omp_v2
run_omp_v2: task_omp_v2.o
	./task_omp_v2.o $(GRID_SIZE) $(GRID_SIZE) $(THREADS_COUNT)

.PHONY: clean
clean:
	rm *.o result_*.txt
