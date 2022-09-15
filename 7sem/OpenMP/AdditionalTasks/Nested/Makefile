all: nested.o

nested.o: nested.cpp
	g++ -std=c++2a nested.cpp -o nested.o -fopenmp

PHONY: clean
clean:
	rm nested.o
