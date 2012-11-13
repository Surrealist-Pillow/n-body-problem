CFLAGS = -std=c99 -Wall -g
CXX = icc
LIBS = -lm

all: nbody holder

nbody: nbody_main.c nbody_prefetch.c nbody.c
	$(CXX) $(CFLAGS) $^ $(LIBS) -o nbody

holder: holder_main.c holder_prefetch.c nbody_holder.c nbody.c
	$(CXX) $(CFLAGS) $^ $(LIBS) -o holder

clean:
	rm -f nbody holder
