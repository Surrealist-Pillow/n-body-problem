#include "nbody.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static clock_t c0, c1;

static void timeRegular(nbody_t* points)
{
  c0 = clock(); 
  computeInteractions(points);
  c1 = clock();
  printf("Elapsed CPU time without sorting is %lf seconds\n",
      (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void timeBlocked(nbody_t* points)
{
  c0 = clock(); 
  computeBlockedInteractions(points);
  c1 = clock();
  printf("Elapsed CPU time with blocking is %lf seconds\n", 
      (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void timeSorted(nbody_t* points)
{
  c0 = clock();
  computeSortedInteractions(points);
  c1 = clock();
  printf("Elapsed CPU time with sorting is %lf\n", 
      (((double)c1)-c0)/CLOCKS_PER_SEC);
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    printf("Usage: ./holder <option>\n");
    printf("Potential options are 'sort', 'block', or 'none'\n");
    exit(1);
  }
  nbody_t* points = getPoints();
  if (!strcmp(argv[1], "sort"))
    timeSorted(points);
  else if (!strcmp(argv[1], "block"))
    timeBlocked(points);
  else if (!strcmp(argv[1], "none"))
    timeRegular(points);
  else {
    printf("Invalid option\n");
    exit(1);
  }
  free(points);
  return 0;
}
