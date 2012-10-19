#ifndef NBODY_H
#define NBODY_H

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

nbody_holder_t* getPoints();
void freePoints(const nbody_holder_t* pointArray);
void printPoints(const nbody_holder_t* points);
int compareHolders(const void* holderA, const void* holderB);
void computeInteractions(const nbody_holder_t* points);
void computeSortedInteractions(nbody_holder_t* points);
void computeForce(nbody_t* first, const nbody_t* second);
void updatePosition(nbody_t* body);

#endif
