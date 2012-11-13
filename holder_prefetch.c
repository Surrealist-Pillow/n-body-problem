#include "nbody_holder.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "prefetch.h"

static const int PREFETCH_AMOUNT = 25;

/*
 * Compute the interactions between the parameter and 
 * all the holder objects.
 */
void computeHolderPrefetchInteractions(nbody_holder_t* points)
{
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
        // Prefetch the element that is at index (i + PREFETCH_AMOUNT)
        __builtin_prefetch(points + (i+PREFETCH_AMOUNT),0,0);
        double dist = abs(points[i].x - points[j].x);
        if (dist < THRESHOLD)
          computeForce(points[i].body, points[j].body);
      }
      updatePosition(points[i].body);
      points[i].x = points[i].body->x;
    }
  }
}

/**
 * Compute interactions using blocking on holder structures rather
 * than full body structures.
 */
void computeHolderBlockedPrefetchInteractions(nbody_holder_t* points)
{
  int BLOCK_SIZE = 10000;
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int l = 0; l < INPUT_SIZE; l += BLOCK_SIZE) 
    {
      for (int k = l; k < INPUT_SIZE; k += BLOCK_SIZE) 
      {
        int maxIndex = MIN(k + BLOCK_SIZE, INPUT_SIZE);
        for (int i = k; i < maxIndex; i++) 
        {
          points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
          for (int j = i+1; j < maxIndex; j++) 
          {
            __builtin_prefetch(points + (i+PREFETCH_AMOUNT),0,0);
            double dist = abs(points[i].x - points[j].x);
            if (dist < THRESHOLD)
              computeForce(points[i].body, points[j].body);
          }
          updatePosition(points[i].body);
          points[i].x = points[i].body->x;
        }
      }
    }
  }
}

void computeHolderSortedPrefetchInteractions(nbody_holder_t* points) 
{
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    // Use qsort to sort the points by x value
    qsort(points, INPUT_SIZE, sizeof(nbody_holder_t), compareHolders);
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
        __builtin_prefetch(points + (i+PREFETCH_AMOUNT),0,0);
        double dist = abs(points[i].x - points[j].x);
        // Once the distance has reached the threshold we know
        // we don't need to compute any further.
        if (dist < THRESHOLD)
          computeForce(points[i].body, points[j].body);
        else break;
      }
      updatePosition(points[i].body);
      points[i].x = points[i].body->x;
    }
  }
}
