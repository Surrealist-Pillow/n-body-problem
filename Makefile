OBJECTS = nbody.o
SOURCES = nbody.h nbody.cpp
CFLAGS = -g -Wall
CXX = g++
LIBS = -lm

all: nbody

nbody: $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) $(LIBS) -o nbody

nbody.o: nbody.cpp
	$(CXX) $(CFLAGS) -c nbody.cpp
clean:
	rm nbody *o

