all: hello_world.o

hello_world.o: hello_world.cpp
	mpic++ hello_world.cpp -o hello_world.o -fopenmp

PHONY: clean
clean:
	rm hello_world.o
