OBJECTS = main.o nbody.o
CFLAGS = -std=c99 -pg -Wall -g
IFLAGS = -opt-report-phase all -opt-report-file nbody.rpt -opt-report 3
CXX = icc
LIBS = -lm

all: nbody

nbody: $(OBJECTS)
	$(CXX) $(CFLAGS) $(IFLAGS) $(OBJECTS) $(LIBS) -o nbody

nbody.o: nbody.c
	$(CXX) $(CFLAGS) $(IFLAGS) -c nbody.c

main.o: main.c
	$(CXX) $(CFLAGS) $(IFLAGS) -c main.c

clean:
	rm nbody *o
