all: car ship caretaker


car: car.cpp
	clang++ car.cpp -o car -Wall -Wextra -lrt -pthread
	
ship: ship.cpp
	clang++ ship.cpp -o ship -Wall -Wextra -lrt -pthread
	
caretaker: caretaker.cpp
	clang++ caretaker.cpp -o caretaker -Wall -Wextra -lrt -pthread
	
PHONY: clean
clean:
	rm car caretaker ship *~
