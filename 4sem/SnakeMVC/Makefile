CC=g++
CFLAGS=-c -std=c++2a -O3
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

SOURCES_CLASSIC=classic.cpp Controllers/keyboard.cpp Views/sfml.cpp Types/block.cpp Types/position.cpp Types/event.cpp Models/classic.cpp
OBJECTS_CLASSIC=$(SOURCES_CLASSIC:.cpp=.o)
GAME_CLASSIC=game_classic

SOURCES_ADVANCED=advanced.cpp Controllers/keyboard.cpp Views/sfml.cpp Types/block.cpp Types/position.cpp Types/event.cpp Models/pvp.cpp
OBJECTS_ADVANCED=$(SOURCES_ADVANCED:.cpp=.o)
GAME_ADVANCED=game_advanced


all: $(GAME_CLASSIC) $(GAME_ADVANCED)


$(GAME_ADVANCED): $(OBJECTS_ADVANCED)
	$(CC) $(LDFLAGS) $(OBJECTS_ADVANCED) -o $@

	
$(GAME_CLASSIC): $(OBJECTS_CLASSIC)
	$(CC) $(LDFLAGS) $(OBJECTS_CLASSIC) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
	
.PHONY: clean
clean:
	rm *.o */*.o
