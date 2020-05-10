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
	rm -rf *.o Source/*.o
	
.PHONY: rm
rm:
	rm -rf *.png *.out *.s
	
.PHONY: run
run:
	./"$(EXECUTABLE)" 
