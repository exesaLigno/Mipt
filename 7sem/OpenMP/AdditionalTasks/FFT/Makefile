all: fft.o

fft.o: main.cpp FFT.cpp FFT_optimized.cpp clocking.cpp
	g++ main.cpp -o fft.o -std=c++2a

.PHONY: clean
clean: 
	rm *.o
