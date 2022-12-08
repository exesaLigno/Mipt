all: hello_world.o

hello_world.o: hello_world.cpp
	g++ hello_world.cpp -o hello_world.o -fopenmp -std=c++2a

PHONY: clean
clean:
	rm hello_world.o
