CC=g++
CFLAGS=-c -Wall -std=c++2a
LDFLAGS=
SOURCES=main.cpp Source/programm.cpp Source/token.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=app.out

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
.PHONY: clean
clean:
	rm -rf *.o Source/*.o app.out
	
.PHONY: run
run:
	./"$(EXECUTABLE)" 
