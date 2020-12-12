all: watch

watch: watch.cpp
	clang++ watch.cpp -o watch -Wall -Wextra -lrt -pthread
	
PHONY: clean
clean:
	rm watch *~
