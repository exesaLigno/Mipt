CC = g++
CFLAGS = -c -std=c++2a -O3 -pthread
LDFLAGS =

SOURCES = integral.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
INTEGRATOR = integrate.out

all: $(INTEGRATOR)

$(INTEGRATOR): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
