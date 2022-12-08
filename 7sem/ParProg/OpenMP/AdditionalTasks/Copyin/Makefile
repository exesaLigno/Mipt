all: copyin.o

copyin.o: copyin_demonstration.cpp
	g++ -std=c++2a copyin_demonstration.cpp -o copyin.o -fopenmp

PHONY: clean
clean:
	rm copyin.o
