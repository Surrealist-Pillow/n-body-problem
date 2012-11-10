#ifndef NBODY_H
#define NBODY_H

#include <stdbool.h>

typedef struct _nbody_t {
  double mass;
  double x;
  double y;
  double z;
  double vx;
  double vy;
  double vz;
  double ax;
  double ay;
  double az;
} nbody_t;

/* Function declarations */
nbody_t* getPoints();
void printPoints(const nbody_t* points);
int compareBodies(const void* bodyA, const void* bodyB);
void computeInteractions(nbody_t* points);
void computeSortedInteractions(nbody_t* points);
void computeBlockedInteractions(nbody_t* points);
void computeForce(nbody_t* first, const nbody_t* second);
void updatePosition(nbody_t* body);

#endif
