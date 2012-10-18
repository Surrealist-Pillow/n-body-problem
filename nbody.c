#include "nbody.h"
#include <stdio>
#include <cmath>

static int INPUT_SIZE, NTIMESTEPS, DTIME, DTHF, EPS;
const double THRESHOLD = 0.5;

int main()
{
  nbody_holder_t* points = getPoints();
  computeInteractions(points);
  freePoints(points);
  return 0;
}

/*
 * Print out the points for a given vector of either 
 * holders or body objects.
 */
template <class T>
void printPoints(const nbody_holder_t* points)
{
  for (int i = 0; i < INPUT_SIZE; i++)
    printf("%g", points[i].x);
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
  int* outputs[] = { &INPUT_SIZE, &NTIMESTEPS, &DTIME, &EPS };
  for (int i = 0; i < 4; i++) {
    fgets(cin, cur_line);
    sscanf(cur_line, "%d", outputs[i]);
  }
  DTHF = 0.5 * DTIME;
  nbody_holder_t* holders = malloc(INPUT_SIZE * sizeof(nbody_holder_t));
  fgets(cur_line, 1024, stdin); // ignore TOL
  int i = 0;
  while (fgets(cur_line, 1024, stdin))
  {
    nbody_t* body = new nbody_t;
    body->ax = 0;
    body->ay = 0;
    body->az = 0;
    sscanf("%G %G %G %G %G %G %G", &(body->mass), &(body->x), &(body->y),
    &(body->z), &(body->vx), &(body->vy), &(body->vz));
    holders[i].x = body->x;
    holders[i].body = body;
    i++;
  }
  return holders;
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
