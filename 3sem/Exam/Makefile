all: task_1 task_2

task_1: task_1.cpp
	clang++ task_1.cpp -o task_1 -Wall -Wextra -lrt -pthread
	
task_2: task_2.cpp
	clang++ task_2.cpp -o task_2 -Wall -Wextra -lrt -pthread
	
PHONY: clean
clean:
	rm task_1 task_2 *~
