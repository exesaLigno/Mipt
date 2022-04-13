all: summ.o

summ.o: summ.cpp
	g++ -o summ.o summ.cpp -pthread

PHONY: clean
clean:
	rm summ.o
