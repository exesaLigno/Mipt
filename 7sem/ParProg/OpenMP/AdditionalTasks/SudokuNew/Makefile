CC=g++
CFLAGS=-c -std=c++2a -O3
LDFLAGS=

SOLVER_SOURCES=main.cpp sudoku.cpp
SOLVER_OBJECTS=$(SOLVER_SOURCES:.cpp=.o)
SOLVER=solver.o
SOLVER_PARALLEL=solver_par.o

all: $(SOLVER) $(SOLVER_PARALLEL)
	
$(SOLVER_PARALLEL): $(SOLVER_OBJECTS)
	$(CC) $(LDFLAGS) $(SOLVER_OBJECTS) -o $@ -fopenmp
	
$(SOLVER): $(SOLVER_OBJECTS)
	# $(CC) $(LDFLAGS) $(SOLVER_OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ -fopenmp
