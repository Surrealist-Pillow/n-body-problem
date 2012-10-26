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

typedef struct _nbody_holder_t {
  double x;
  nbody_t* body;
} nbody_holder_t;

typedef struct _nbody_type_t {
  int type;
  union {
    nbody_t* bodies;
    nbody_holder_t* holders;
  };
} nbody_type_t;

typedef struct _interact_param_t {
  int index;
  int max;
  bool sorted;
} interact_param_t;

/* Function declarations */
nbody_type_t* getPoints(int type);
void freePoints(const nbody_type_t* pointArray);
void printPoints(const nbody_type_t* points);
int compareHolders(const void* holderA, const void* holderB);
int compareBodies(const void* bodyA, const void* bodyB);
void computeInteractions(const nbody_type_t* points);
void computeSortedInteractions(nbody_type_t* points);
void computeBlockedInteractions(const nbody_type_t* points);
void computeForce(nbody_t* first, const nbody_t* second);
void updatePosition(nbody_t* body);
void interact(const nbody_type_t* points, const interact_param_t* param);
void setAccelerations(const nbody_type_t* points, int index);
void sortPoints(nbody_type_t* points);

#endif
