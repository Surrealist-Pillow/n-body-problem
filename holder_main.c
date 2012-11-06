#include "nbody_holder.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
  nbody_holder_t* holders = getHolders();
  time_t  t0, t1;
  clock_t c0, c1;
  // Compute interactions between the points and time it takes.
  t0 = time(NULL);
  c0 = clock(); 
  computeHolderInteractions(holders);
  t1 = time(NULL);
  c1 = clock();
  printf("Elapsed clock time without improvements is %lf seconds\n", (double)t1-t0);
  printf("Elapsed CPU time without improvements is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
  // Now to test blocking!
  t0 = time(NULL);
  c0 = clock(); 
  computeHolderBlockedInteractions(holders);
  t1 = time(NULL);
  c1 = clock();
  printf("Elapsed clock time with blocking is %lf seconds\n", (double)t1-t0);
  printf("Elapsed CPU time with blocking is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
  // Now for the sorted algorithm (which should be faster).
  t0 = time(NULL);
  c0 = clock();
  computeHolderSortedInteractions(holders);
  t1 = time(NULL);
  c1 = clock();
  printf("Elapsed wall clock time with sorting is %lf seconds\n", (double)t1-t0);
  printf("Elapsed CPU time with sorting is %lf\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
  freePoints(holders);
  free(holders);
  return 0;
}
