CFLAGS = -std=c99 -Wall -g
#IFLAGS = -opt-report-phase all -opt-report-file nbody.rpt -opt-report 3
IFLAGS =
CXX = gcc
LIBS = -lm

all: nbody holder

nbody: main.c nbody.c
	$(CXX) $(CFLAGS) $(IFLAGS) main.c nbody.c $(LIBS) -o nbody

holder: holder_main.c nbody_holder.c nbody.c
	$(CXX) $(CFLAGS) $(IFLAGS) holder_main.c nbody_holder.c nbody.c $(LIBS) -o holder

clean:
	rm -f nbody holder
