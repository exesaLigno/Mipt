CC=g++
CFLAGS=-c -std=c++2a -O3
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

SOURCES=main.cpp Controllers/keyboard.cpp Models/classic.cpp Views/sfml.cpp Types/block.cpp Types/position.cpp
OBJECTS=$(SOURCES:.cpp=.o)
GAME=game


all: $(GAME)
	
$(GAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
	
.PHONY: clean
clean:
	rm *.o */*.o
