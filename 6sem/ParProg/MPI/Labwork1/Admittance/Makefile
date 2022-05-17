all: admittance.out

admittance.out: admittance.cpp
	mpic++ -o admittance.out admittance.cpp

PHONY: run
run: admittance.out
	mpiexec -n 4 admittance.out

PHONY: clean
clean:
	rm admittance.out
