#include "nbody_holder.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Print out the points for a given vector of either 
 * holders or body objects.
 */
void printHolders(const nbody_holder_t* points)
{
  int i;
  for (i = 0; i < INPUT_SIZE; i++)
  {
     printf("%lf %lf %lf\n", points[i].x,
        points[i].body->y, points[i].body->z);
  }
}

void freePoints(nbody_holder_t* points) {
  int i;
  for (i = 0; i < INPUT_SIZE; i++) 
  {
    free(points[i].body);
    points[i].body = NULL;
  }
}

/*
 * Returns a vector of n_body structures containing each
 * of the points contained in the input file.
 */
nbody_holder_t* getHolders()
{
  char cur_line[1024];
  int* outputs[] = { &INPUT_SIZE, &NTIMESTEPS };
  double* doubles[] = { &DTIME, &EPS };
  for (int i = 0; i < 4; i++) 
  {
    fgets(cur_line, 1024, stdin);
    if (i < 2)
      sscanf(cur_line, "%d", outputs[i]);
    else
      sscanf(cur_line, "%lf", doubles[i-2]);
  }
  DTHF = 0.5 * DTIME;
  nbody_holder_t* retVal = malloc(INPUT_SIZE * sizeof(nbody_holder_t));
  fgets(cur_line, 1024, stdin); // ignore TOL
  int i = 0;
  while (fgets(cur_line, 1024, stdin))
  {
    retVal[i].body = malloc(sizeof(nbody_t));
    retVal[i].body->ax = 0;
    retVal[i].body->ay = 0;
    retVal[i].body->az = 0;
    sscanf(cur_line,"%lf %lf %lf %lf %lf %lf %lf",
        &(retVal[i].body->mass), &(retVal[i].x), &(retVal[i].body->y),
        &(retVal[i].body->z), &(retVal[i].body->vx), &(retVal[i].body->vy),
        &(retVal[i].body->vz));
    retVal[i].body->x = retVal[i].x;
    i++;
  }
  return retVal;
}

/*
 * Comparator function passed to qsort in order to compare
 * two holder objects.
 */
int compareHolders(const void* holderA, const void* holderB)
{
  const nbody_holder_t* hold_A = holderA;
  const nbody_holder_t* hold_B = holderB;
  return hold_A->x - hold_B->x;
}

/*
 * Compute the interactions between the parameter and 
 * all the holder objects.
 */
void computeHolderInteractions(nbody_holder_t* points)
{
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
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
void computeHolderBlockedInteractions(nbody_holder_t* points)
{
  int BLOCK_SIZE = 4096;
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int l = 0; l < INPUT_SIZE; l += BLOCK_SIZE)
    {
      for (int i=l; i<MIN(l+BLOCK_SIZE, INPUT_SIZE); i++)
        points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
      for (int k = l; k < INPUT_SIZE; k += BLOCK_SIZE) 
      {
        int maxIndexi = MIN(k + BLOCK_SIZE, INPUT_SIZE);
        if (k == l)
        {
          for (int i = l; i < maxIndexi; i++) 
          {
            int maxIndexj = MIN(k + BLOCK_SIZE, INPUT_SIZE);
            for (int j = i+1; j < maxIndexj; j++) 
            {
              double dist = abs(points[i].x - points[j].x);
              if (dist < THRESHOLD)
                computeForce(points[i].body, points[j].body);
            }
          }
        }
        else
        {
          for (int i = l; i < maxIndexi; i++) 
          { 
            int maxIndexj = MIN(k + BLOCK_SIZE, INPUT_SIZE);
            for (int j = k; j < maxIndexj; j++) 
            {
              double dist = abs(points[i].x - points[j].x);
              if (dist < THRESHOLD) 
                computeForce(points[i].body, points[j].body);
            }
          }
        }
      }
      for (int i=l; i<MIN(l+BLOCK_SIZE, INPUT_SIZE); i++)
      {
          updatePosition(points[i].body);
          points[i].x = points[i].body->x;
      }
    }
  }
}

void computeHolderSortedInteractions(nbody_holder_t* points) {
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    // Use qsort to sort the points by x value
    qsort(points, INPUT_SIZE, sizeof(nbody_holder_t), compareHolders);
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
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
