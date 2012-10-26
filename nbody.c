#include "nbody.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "globals.h"

/*
 * Print out the points for a given vector of either 
 * holders or body objects.
 */
void printPoints(const nbody_type_t* points)
{
  int i;
  for (i = 0; i < INPUT_SIZE; i++)
  {
    if (points->type == 1) {
      printf("%lf %lf %lf\n", points->holders[i].x, 
        points->holders[i].body->y, points->holders[i].body->z);
    }
    else {
     printf("%lf %lf %lf\n", points->bodies[i].x, 
        points->bodies[i].y, points->bodies[i].z); 
    }
  }
}

void freePoints(const nbody_type_t* pointArray)
{
  if (pointArray->type == 1) {
    for (int i = 0; i < INPUT_SIZE; i++)
      free(pointArray->holders[i].body);
    free(pointArray->holders);
  }
  else free(pointArray->bodies);
}

/*
 * Returns a vector of holder structs. This supports good
 * cache performance by only storing the x value in the 
 * holding structure and containing a pointer to the rest.
 */
nbody_type_t* getPoints(int type)
{
  char cur_line[1024];
  int* outputs[] = { &INPUT_SIZE, &NTIMESTEPS };
  double* doubles[] = { &DTIME, &EPS };
  for (int i = 0; i < 4; i++) {
    fgets(cur_line, 1024, stdin);
    if (i < 2)
      sscanf(cur_line, "%d", outputs[i]);
    else
      sscanf(cur_line, "%lf", doubles[i-2]);
  }
  DTHF = 0.5 * DTIME;
  nbody_type_t* retVal = malloc(sizeof(nbody_type_t));
  retVal->type = type;
  if (type == 1)
    retVal->holders = malloc(INPUT_SIZE * sizeof(nbody_holder_t));
  else
    retVal->bodies = malloc(INPUT_SIZE * sizeof(nbody_t));
  fgets(cur_line, 1024, stdin); // ignore TOL
  int i = 0;
  while (fgets(cur_line, 1024, stdin))
  {
    nbody_t* bodyPtr, body;
    if (type == 1)
      bodyPtr = malloc(sizeof(nbody_t));
    else bodyPtr = &body;
    bodyPtr->ax = 0;
    bodyPtr->ay = 0;
    bodyPtr->az = 0;
    sscanf(cur_line,"%lf %lf %lf %lf %lf %lf %lf",
        &(bodyPtr->mass),&(bodyPtr->x),&(bodyPtr->y),&(bodyPtr->z),
        &(bodyPtr->vx),&(bodyPtr->vy),&(bodyPtr->vz));
    if (type == 1) {
      retVal->holders[i].x = bodyPtr->x;
      retVal->holders[i].body = bodyPtr;
    }
    else {
      retVal->bodies[i] = body;
    }
    i++;
  }
  return retVal;
}

int compareHolders(const void* holderA, const void* holderB)
{
  const nbody_holder_t* hold_A = holderA;
  const nbody_holder_t* hold_B = holderB;
  return hold_A->x - hold_B->x;
}

int compareBodies(const void* bodyA, const void* bodyB)
{
  const nbody_t* body_A = bodyA;
  const nbody_t* body_B = bodyB;
  return body_A->x - body_B->x;
}

void setAccelerations(const nbody_type_t* points, int index)
{
    // Need separate handling for holders and bodies
    if (points->type == 1)
      points->holders[index].body->ax = points->holders[index].body->ay = points->holders[index].body->az = 0;
    else 
      points->bodies[index].ax = points->bodies[index].ay = points->bodies[index].az = 0;
}

void computeInteractions(const nbody_type_t* points)
{    
  interact_param_t parameters;
  parameters.max = INPUT_SIZE;
  parameters.sorted = false;
  for (int step = 0; step < NTIMESTEPS; step++) {
    for (int i = 0; i < INPUT_SIZE; i++) {
      setAccelerations(points, i);
      printf("%d\n", i);
      parameters.index = i;
      interact(points, &parameters);
    }
  }
}

void interact(const nbody_type_t* points, const interact_param_t* param)
{
  int index = param->index;
  bool sorted = param->sorted;
  // If we have a array of holders, process in one way.
  if (points->type == 1) {
    for (int j = index+1; j < param->max; j++) {
      double dist = abs(points->holders[index].x - points->holders[j].x);
      if (dist < THRESHOLD)
        computeForce(points->holders[index].body, points->holders[j].body);
      else {
        if (sorted)
          continue;
      }
    }
    updatePosition(points->holders[index].body);
  }
  // Else it's an array of body objects, process differently.
  else {
    for (int j = index+1; j < param->max; j++) {
      double dist = abs(points->bodies[index].x - points->bodies[j].x);
      if (dist < THRESHOLD)
        computeForce(&points->bodies[index], &points->bodies[j]);
      else {
        if (sorted)
          continue;
      }
    }
    updatePosition(&points->bodies[index]);
  }
}

void computeBlockedInteractions(const nbody_type_t* points)
{
  interact_param_t parameters;
  parameters.sorted = false;
  int BLOCK_SIZE = 4096;
  for (int step = 0; step < NTIMESTEPS; step++) {
    for (int l = 0; l < INPUT_SIZE; l += BLOCK_SIZE) {
      for (int k = l; k < INPUT_SIZE; k += BLOCK_SIZE) {
        int maxIndex = MIN(k + BLOCK_SIZE, INPUT_SIZE);
        parameters.max = maxIndex;
        for (int i = 0; i < maxIndex; i++) {
          setAccelerations(points, i);
          parameters.index = i;
          interact(points, &parameters);
        }
      }
    }
  } 
}

void computeSortedInteractions(nbody_type_t* points) {
  interact_param_t parameters;
  parameters.max = INPUT_SIZE;
  parameters.sorted = true;
  // Use qsort to sort the points by x value
  for (int step = 0; step < NTIMESTEPS; step++) {
    sortPoints(points);
    for (int i = 0; i < INPUT_SIZE; i++) {
      setAccelerations(points, i);
      parameters.index = i;
      interact(points, &parameters);
    }
  }
}

void sortPoints(nbody_type_t* points)
{
  if (points->type == 1)
    qsort(points->holders, INPUT_SIZE, sizeof(nbody_holder_t), compareHolders);
  else 
    qsort(points->bodies, INPUT_SIZE, sizeof(nbody_t), compareBodies);
}

// computes the acceleration delta for this particular interaction
void computeForce(nbody_t* first, const nbody_t* second)
{
  double xDist = second->x - first->x;
  double yDist = second->y - first->y;
  double zDist = second->z - first->z;
  
  double rSquared = xDist * xDist + yDist * yDist + zDist * zDist;
  double coefficient = second->mass / pow(rSquared, 1.5);
  
  first->ax += coefficient * xDist;
  first->ay += coefficient * yDist;
  first->az += coefficient * zDist;
}

// computes the final position for a body
void updatePosition(nbody_t* body)
{
  double halfXDelta = body->ax * DTHF;
  double halfYDelta = body->ay * DTHF;
  double halfZDelta = body->az * DTHF;
  
  double meanXVel = body->vx + halfXDelta;
  double meanYVel = body->vy + halfYDelta;
  double meanZVel = body->vz + halfZDelta;
  
  body->x += meanXVel * DTIME;
  body->y += meanYVel * DTIME;
  body->z += meanZVel * DTIME;
  
  body->vx = meanXVel + halfXDelta;
  body->vy = meanYVel + halfYDelta;
  body->vz = meanZVel + halfZDelta;
}
