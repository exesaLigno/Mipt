all: plots main.o

main.o: V4.cpp
	g++ -std=c++2a V4.cpp -o main.o -fopenmp

plots: main.o
	./main.o 10000
	./plotter.py V4_output_linear.txt
	./plotter.py V4_output_parallel.txt

compare: main.o
	echo "┌─────────────────┬─────────────────┬─────────────────┐"
	echo "│        N        │   Linear, mks   │  Parallel, mks  │"
	echo "├─────────────────┼─────────────────┼─────────────────┤"
	./main.o 100 -s
	./main.o 1000 -s
	./main.o 10000 -s
	./main.o 100000 -s
	./main.o 1000000 -s
	./main.o 2000000 -s
	./main.o 2500000 -s
	./main.o 2750000 -s
	./main.o 2900000 -s
	./main.o 3000000 -s
	echo "└─────────────────┴─────────────────┴─────────────────┘"

clean:
	rm *.txt *.png *.o
