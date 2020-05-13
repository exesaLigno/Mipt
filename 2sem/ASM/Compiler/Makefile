CC=g++
CFLAGS=-c -Wall -std=c++2a -O3
LDFLAGS=
SOURCES=main.cpp Source/settings.cpp Source/programm.cpp Source/token.cpp Source/library.cpp Source/definition.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jc

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
.PHONY: clean
clean:
	rm -rf *.o Source/*.o $(EXECUTABLE)
	
.PHONY: rm
rm:
	rm -rf *.png *.out *.s *.out
	
.PHONY: run
run:
	./"$(EXECUTABLE)" 
	
.PHONY: nasm
nasm:
	nasm -f elf64 test.s -o test.o
	ld -s -o a.out test.o
