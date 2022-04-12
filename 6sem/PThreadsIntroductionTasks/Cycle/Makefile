all: cycle.o

cycle.o: cycle.cpp
	g++ -o cycle.o cycle.cpp -pthread

PHONY: clean
clean:
	rm cycle.o
