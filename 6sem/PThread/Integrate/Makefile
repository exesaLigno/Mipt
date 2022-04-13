all: integrate.o

integrate.o: integrate.cpp
	g++ -o integrate.o integrate.cpp -pthread

PHONY: clean
clean:
	rm integrate.o
