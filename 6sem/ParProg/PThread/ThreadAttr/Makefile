all: attr.o

attr.o: attr.cpp
	g++ -o attr.o attr.cpp -pthread

PHONY: clean
clean:
	rm attr.o
