#include "nbody.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "globals.h"

int INPUT_SIZE = 0, NTIMESTEPS = 0; 
double DTIME = 0, DTHF = 0, EPS = 0;
const double THRESHOLD = 0.5;

/*
 * Print out the points for a given vector of either 
 * holders or body objects.
 */
void printPoints(const nbody_t* points)
{
  int i;
  for (i = 0; i < INPUT_SIZE; i++)
  {
     printf("%lf %lf %lf\n", points[i].x,
        points[i].y, points[i].z);
  }
}

/*
 * Returns a vector of n_body structures containing each
 * of the points contained in the input file.
 */
nbody_t* getPoints()
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
  nbody_t* retVal = malloc(INPUT_SIZE * sizeof(nbody_t));
  fgets(cur_line, 1024, stdin); // ignore TOL
  int i = 0;
  while (fgets(cur_line, 1024, stdin))
  {
    retVal[i].ax = 0;
    retVal[i].ay = 0;
    retVal[i].az = 0;
    sscanf(cur_line,"%lf %lf %lf %lf %lf %lf %lf",
        &(retVal[i].mass), &(retVal[i].x), &(retVal[i].y), &(retVal[i].z),
        &(retVal[i].vx), &(retVal[i].vy), &(retVal[i].vz));
    i++;
  }
  return retVal;
}

int compareBodies(const void* bodyA, const void* bodyB)
{
  const nbody_t* body_A = bodyA;
  const nbody_t* body_B = bodyB;
  return body_A->x - body_B->x;
}

void computeInteractions(nbody_t* points)
{
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].ax = points[i].ay = points[i].az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
        double dist = abs(points[i].x - points[j].x);
        if (dist < THRESHOLD)
          computeForce(&points[i], &points[j]);
      }
      updatePosition(&points[i]);
    }
  }
}

void computeBlockedInteractions(nbody_t* points)
{
  int BLOCK_SIZE = 4096;
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    for (int l = 0; l < INPUT_SIZE; l += BLOCK_SIZE)
    {
      for (int i = l; i < MIN(l + BLOCK_SIZE, INPUT_SIZE); i++)
        points[i].ax = points[i].ay = points[i].az = 0;
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
                computeForce(&points[i], &points[j]);
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
                computeForce(&points[i], &points[j]);
            }
          }
        }
      }
      for (int i = l; i < MIN(l + BLOCK_SIZE, INPUT_SIZE); i++)
          updatePosition(&points[i]);
    }
  }
}

void computeSortedInteractions(nbody_t* points) {
  // Use qsort to sort the points by x value
  for (int step = 0; step < NTIMESTEPS; step++) 
  {
    qsort(points, INPUT_SIZE, sizeof(nbody_t), compareBodies);
    for (int i = 0; i < INPUT_SIZE; i++) 
    {
      points[i].ax = points[i].ay = points[i].az = 0;
      for (int j = i+1; j < INPUT_SIZE; j++) 
      {
        double dist = abs(points[i].x - points[j].x);
        if (dist < THRESHOLD)
          computeForce(&points[i], &points[j]);
        else break;
      }
      updatePosition(&points[i]);
    }
  }
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
