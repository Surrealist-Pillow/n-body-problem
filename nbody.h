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
/* 
 *  templatized methods that can take in either a vector of points 
 *  or a vector of point holders.
 */
template <class T>
void printPoints(const vector<T>& points);
template <class T>
void computeCollisions(const vector<T>& points);
template <class T>
void computeNewVelocities(const T& first, const T& second);

#endif
