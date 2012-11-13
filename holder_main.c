#include "nbody_holder.h"
#include "prefetch.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static clock_t c0, c1;



static void noImprovements(nbody_holder_t* holders)
{
  c0 = clock(); 
  computeHolderInteractions(holders);
  c1 = clock();
  printf("Elapsed CPU time without improvements is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void noImprovementsPrefetch(nbody_holder_t* holders)
{
  c0 = clock(); 
  computeHolderPrefetchInteractions(holders);
  c1 = clock();
  printf("Elapsed CPU time without improvements and with prefetching is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void withBlocking(nbody_holder_t* holders)
{
  c0 = clock(); 
  computeHolderBlockedInteractions(holders);
  c1 = clock();
  printf("Elapsed CPU time with blocking is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void withBlockingPrefetch(nbody_holder_t* holders)
{
  c0 = clock(); 
  computeHolderBlockedPrefetchInteractions(holders);
  c1 = clock();
  printf("Elapsed CPU time with blocking and prefetching is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void withSorting(nbody_holder_t* holders)
{
  c0 = clock();
  computeHolderSortedInteractions(holders);
  c1 = clock();
  printf("Elapsed CPU time with sorting is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
}

static void withSortingPrefetch(nbody_holder_t* holders)
{
  c0 = clock();
  computeHolderSortedPrefetchInteractions(holders);
  c1 = clock();
  printf("Elapsed CPU time with sorting and prefetching is %lf seconds\n", (((double)c1)-c0)/CLOCKS_PER_SEC);
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
  nbody_holder_t* holders = getHolders();
  if (argv[2] && !strcmp(argv[2], "prefetch")) 
  {
    if (!strcmp(argv[1], "sort"))
      withSortingPrefetch(holders);
    else if (!strcmp(argv[1], "block"))
      withBlockingPrefetch(holders);
    else if (!strcmp(argv[1], "none"))
      noImprovementsPrefetch(holders);
    else
      printf("Invalid option\n");
  }
  else 
  {
    if (!strcmp(argv[1], "sort"))
      withSorting(holders);
    else if (!strcmp(argv[1], "block"))
      withBlocking(holders);
    else if (!strcmp(argv[1], "none"))
      noImprovements(holders);
    else
      printf("Invalid option\n");
  }
  freePoints(holders);
  free(holders);
  return 0;
}
