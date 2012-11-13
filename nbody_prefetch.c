#include "nbody.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "globals.h"
#include "prefetch.h"

static const int PREFETCH_AMOUNT = 25;

void computePrefetchInteractions(nbody_t* points)
{
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].ax = points[i].ay = points[i].az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
        __builtin_prefetch(points + (i+PREFETCH_AMOUNT),0,0);
        double dist = abs(points[i].x - points[j].x);
        if (dist < THRESHOLD)
          computeForce(&points[i], &points[j]);
      }
      updatePosition(&points[i]);
    }
  }
}

void computeBlockedPrefetchInteractions(nbody_t* points)
{
  int BLOCK_SIZE = 4096;
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int l = 0; l < INPUT_SIZE; l += BLOCK_SIZE) 
    {
      for (int k = l; k < INPUT_SIZE; k += BLOCK_SIZE) 
      {
        int maxIndex = MIN(k + BLOCK_SIZE, INPUT_SIZE);
        for (int i = k; i < maxIndex; i++) 
        {
          points[i].ax = points[i].ay = points[i].az = 0;
          for (int j = i+1; j < maxIndex; j++) 
          {
            __builtin_prefetch(points + (i+PREFETCH_AMOUNT),0,0);
            double dist = abs(points[i].x - points[j].x);
            if (dist < THRESHOLD)
              computeForce(&points[i], &points[j]);
          }
          updatePosition(&points[i]);
        }
      }
    }
  }
}

void computeSortedPrefetchInteractions(nbody_t* points) 
{
  // Use qsort to sort the points by x value
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    qsort(points, INPUT_SIZE, sizeof(nbody_t), compareBodies);
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].ax = points[i].ay = points[i].az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
        __builtin_prefetch(points + (i+PREFETCH_AMOUNT),0,0);
        double dist = abs(points[i].x - points[j].x);
        if (dist < THRESHOLD)
          computeForce(&points[i], &points[j]);
        else break;
      }
      updatePosition(&points[i]);
    }
  }
}