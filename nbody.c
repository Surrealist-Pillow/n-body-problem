#include "nbody.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MIN(X,Y) (X < Y ? X : Y)
#define MAX(X,Y) (X < Y ? Y : X)

static int INPUT_SIZE, NTIMESTEPS; 
static double DTIME, DTHF, EPS;
const double THRESHOLD = 0.5;

int main()
{
  nbody_holder_t* points = getPoints();
  computeInteractions(points);
  printPoints(points);
  freePoints(points);
  free(points);
  return 0;
}

/*
 * Print out the points for a given vector of either 
 * holders or body objects.
 */
void printPoints(const nbody_holder_t* points)
{
  int i;
  for (i = 0; i < INPUT_SIZE; i++)
  {
    printf("%lf %lf %lf\n", points[i].x, 
        points[i].body->y, points[i].body->z);
  }
}

void freePoints(const nbody_holder_t* pointArray)
{
  for (int i = 0; i < INPUT_SIZE; i++)
    free(pointArray[i].body);
}

/*
 * Returns a vector of holder structs. This supports good
 * cache performance by only storing the x value in the 
 * holding structure and containing a pointer to the rest.
 */
nbody_holder_t* getPoints()
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
  nbody_holder_t* holders = malloc(INPUT_SIZE * sizeof(nbody_holder_t));
  fgets(cur_line, 1024, stdin); // ignore TOL
  int i = 0;
  while (fgets(cur_line, 1024, stdin))
  {
    nbody_t* body = malloc(sizeof(nbody_t));
    body->ax = 0;
    body->ay = 0;
    body->az = 0;
    sscanf(cur_line,"%lf %lf %lf %lf %lf %lf %lf",
        &(body->mass),&(body->x),&(body->y),&(body->z),
        &(body->vx),&(body->vy),&(body->vz));
    holders[i].x = body->x;
    holders[i].body = body;
    i++;
  }
  return holders;
}

int compareHolders(const void* holderA, const void* holderB)
{
  const nbody_holder_t* hold_A = holderA;
  const nbody_holder_t* hold_B = holderB;
  if (hold_A->x < hold_B->x) {
    return -1;
  }
  else if (hold_A->x == hold_B->x) {
    return 0;
  }
  else {
    return 1;
  }
}

void computeInteractions(const nbody_holder_t* points)
{    
  for (int step = 0; step < NTIMESTEPS; step++) {
    for (int i = 0; i < INPUT_SIZE; i++) {
      points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
      for(int j = i+1; j < INPUT_SIZE; j++) {
        double dist = abs(points[i].x - points[j].x);
        if (dist < THRESHOLD) {
          computeForce(points[i].body, points[j].body);
        }
      }
      updatePosition(points[i].body);
    }
  }
}

void computeSortedInteractions(nbody_holder_t* points) {
  // Use qsort to sort the points by x value
  for (int step = 0; step < NTIMESTEPS; step++) {
    qsort(points, INPUT_SIZE, sizeof(nbody_holder_t), compareHolders);
    for (int i = 0; i < INPUT_SIZE; i++) {
      points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
      int j = MAX(i-1, 0), k = MIN(i+1, INPUT_SIZE-1);
      double dist1 = abs(points[i].x - points[j].x);
      double dist2 = abs(points[i].x - points[k].x);
      // Since it's sorted, we compute distance only with
      // neighbors
      if (dist1 != 0 && dist1 < THRESHOLD) {
        computeForce(points[i].body, points[j].body);
      }
      if (dist2 != 0 && dist2 < THRESHOLD) {
        computeForce(points[i].body, points[k].body);
      }
      updatePosition(points[i].body);
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
