#include "nbody.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "prefetch.h"

static clock_t c0, c1;

static void timeRegular(nbody_t* points)
{
  c0 = clock(); 
  computeInteractions(points);
  c1 = clock();
  printf("Elapsed CPU time with no improvements is %lf seconds\n",
      (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void timeRegularPrefetch(nbody_t* points)
{
  c0 = clock(); 
  computePrefetchInteractions(points);
  c1 = clock();
  printf("Elapsed CPU time with no improvements and prefetching is %lf seconds\n",
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

static void timeBlockedPrefetch(nbody_t* points)
{
  c0 = clock(); 
  computeBlockedPrefetchInteractions(points);
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

static void timeSortedPrefetch(nbody_t* points)
{
  c0 = clock();
  computeSortedPrefetchInteractions(points);
  c1 = clock();
  printf("Elapsed CPU time with sorting is %lf\n", 
      (((double)c1)-c0)/CLOCKS_PER_SEC);
}

int main(int argc, char* argv[])
{
  if (argc < 2) 
  {
    printf("Usage: ./holder <option> [prefetch]\n");
    printf("Potential options are 'sort', 'block', or 'none'\n");
    printf("For the third option, you can either specify 'prefetch' or leave it blank.\n");
    exit(1);
  }
  nbody_t* points = getPoints();
  if (argv[2] && !strcmp(argv[2], "prefetch")) 
  {
    if (!strcmp(argv[1], "sort"))
      timeSortedPrefetch(points);
    else if (!strcmp(argv[1], "block"))
      timeBlockedPrefetch(points);
    else if (!strcmp(argv[1], "none"))
      timeRegularPrefetch(points);
    else
      printf("Invalid option\n");
  }
  else
  {
    if (!strcmp(argv[1], "sort"))
      timeSorted(points);
    else if (!strcmp(argv[1], "block"))
      timeBlocked(points);
    else if (!strcmp(argv[1], "none"))
      timeRegular(points);
    else
      printf("Invalid option\n");
  }
  free(points);
  return 0;
}
