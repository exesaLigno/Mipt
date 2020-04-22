CC=g++
CFLAGS=-c -Wall -std=c++2a
LDFLAGS=
SOURCES=main.cpp Source/settings.cpp Source/programm.cpp Source/token.cpp
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
	
.PHONY: run
run:
	./"$(EXECUTABLE)" 
