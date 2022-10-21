all: fft.o

fft.o: main.cpp lib/FFT.cpp lib/complex.cpp lib/clocking.cpp
	g++ main.cpp -o fft.o -std=c++2a -fopenmp

.PHONY: clean
clean: 
	rm *.o
