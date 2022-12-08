all: summ.o summ_new.o

summ.o: summ.cpp
	g++ summ.cpp -o summ.o -fopenmp

summ_new.o: summ_new.cpp
	g++ summ_new.cpp -o summ_new.o -fopenmp

PHONY: clean
clean:
	rm summ.o
