all: balancing.o

balancing.o: for_balancing.cpp
	g++ -std=c++2a for_balancing.cpp -o balancing.o -fopenmp

PHONY: clean
clean:
	rm balancing.o
