#include "nbody.h"
#define OWNER 1
#include "globals.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int INPUT_SIZE  = 0, NTIMESTEPS = 0; 
double DTIME = 0, DTHF = 0, EPS = 0;
const double THRESHOLD = 0.5;

static void operateOnStruct(int type) 
{
  nbody_type_t* points = getPoints(type);
  time_t  t0, t1;
  clock_t c0, c1;
  // Compute interactions between the points and time it takes.
  t0 = time(NULL);
  c0 = clock(); 
  computeInteractions(points);
  t1 = time(NULL);
  c1 = clock();
  printf("Elapsed clock time without sorting is %lf seconds\n", (double)t1-t0);
  printf("Elapsed CPU time without sorting is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
  // Now to test blocking!
  t0 = time(NULL);
  c0 = clock(); 
//  computeBlockedInteractions(points);
  t1 = time(NULL);
  c1 = clock();
  printf("Elapsed clock time with blocking is %lf seconds\n", (double)t1-t0);
  printf("Elapsed CPU time with blocking is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
  // Now for the sorted algorithm (which should be faster).
  t0 = time(NULL);
  c0 = clock();
//  computeSortedInteractions(points);
  t1 = time(NULL);
  c1 = clock();
  printf("Elapsed wall clock time with sorting is %lf seconds\n", (double)t1-t0);
  printf("Elapsed CPU time with sorting is %lf\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
  freePoints(points);
  free(points);
}

int main(int argc, char* argv[])
{
  int type;
  if (argc < 2) {
    printf("A running mode is needed. Run this with either \"./nbody holder\" or \"./nbody body\"\n");
    return -1;
  }
  if (!strcmp(argv[1], "holder"))
    type = 1;
  else if (!strcmp(argv[1], "body"))
    type = 0;
  else {
    printf("Invalid option, dude\n");
    return -1;
  }
  operateOnStruct(type);
  return 0;
}

