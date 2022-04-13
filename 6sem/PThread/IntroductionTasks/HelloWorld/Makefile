all: hello.o

hello.o: hello.cpp
	g++ -o hello.o hello.cpp -pthread

PHONY: clean
clean:
	rm hello.o
