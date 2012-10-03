#include <vector>

#ifndef NBODY_H
#define NBODY_H

using namespace std;

typedef struct _nbody_t {
  double mass;
  double x;
  double y;
  double z;
  double vx;
  double vy;
  double vz;
  unsigned long int ident;
} nbody_t;

typedef struct _nbody_holder_t {
  double x;
  nbody_t* body;
} nbody_holder_t;

vector<nbody_holder_t> getPoints();
void freePoints(const vector<nbody_holder_t>& pointArray);
void printPoints(const vector<nbody_holder_t>& points);
void computeCollisions(const vector<nbody_holder_t>& points);
void computeNewVelocities(const nbody_holder_t& first, const nbody_holder_t& second);

#endif
