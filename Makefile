OBJECTS = nbody.o
SOURCES = nbody.h nbody.c
CFLAGS = -std=c99 -g -Wall
CXX = gcc
LIBS = -lm

all: nbody

nbody: $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) $(LIBS) -o nbody

nbody.o: nbody.c
	$(CXX) $(CFLAGS) -c nbody.c
clean:
	rm nbody *o

