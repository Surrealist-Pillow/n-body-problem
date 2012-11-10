CFLAGS = -std=c99 -Wall -g
CXX = icc
LIBS = -lm

all: nbody holder

nbody: main.c nbody.c
	$(CXX) $(CFLAGS) main.c nbody.c $(LIBS) -o nbody

holder: holder_main.c nbody_holder.c nbody.c
	$(CXX) $(CFLAGS) holder_main.c nbody_holder.c nbody.c $(LIBS) -o holder

clean:
	rm -f nbody holder
