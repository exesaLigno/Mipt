all: lab.out lab_np.out

lab.out: lab.cpp
	mpic++ -o lab.out lab.cpp

PHONY: run
run: lab.out
	mpiexec -n 4 ./lab.out

lab_np.out: lab_np.cpp
	g++ -o lab_np.out lab_np.cpp

PHONY: run_np
run_np: lab_np.out
	./lab_np.out

PHONY: compare
compare: lab.out lab_np.out
	mpiexec -n 4 ./lab.out
	./lab_np.out
	./compare.sh result.txt result_np.txt

PHONY: compare2
compare2: lab.out
	mpiexec -n 1 ./lab.out
	mpiexec -n 2 ./lab.out
	mpiexec -n 3 ./lab.out
	mpiexec -n 4 ./lab.out

PHONY: clean
clean:
	rm lab.out lab_np.out result.txt result_np.txt result.png

PHONY: plot
plot: run
	./make_plot.py
