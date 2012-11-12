CFLAGS = -std=c99 -Wall -g
CXX = icc
LIBS = -lm

all: nbody holder nbody_prefetch holder_prefetch

nbody: nbody_main.c nbody.c
	$(CXX) $(CFLAGS) $^ $(LIBS) -o nbody

holder: holder_main.c nbody_holder.c nbody.c
	$(CXX) $(CFLAGS) $^ $(LIBS) -o holder

nbody_prefetch: nbody_main.c nbody_prefetch.c nbody.c
	$(CXX) $(CFLAGS) $^ $(LIBS) -o nbody_prefetch

holder_prefetch: holder_main.c holder_prefetch.c nbody.c
	$(CXX) $(CFLAGS) $^ $(LIBS) -o holder_prefetch

clean:
	rm -f nbody holder
