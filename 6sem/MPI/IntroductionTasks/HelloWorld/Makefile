all: hello_world.o

hello_world.o: hello_world.cpp
	mpic++ hello_world.cpp -o hello_world.o

PHONY: run
run: hello_world.o
	mpiexec -n 4 ./hello_world.o

PHONY: clean
clean:
	rm hello_world.o
