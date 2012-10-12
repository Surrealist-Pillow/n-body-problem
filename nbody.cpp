#include "nbody.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

static int INPUT_SIZE, NTIMESTEPS, DTIME, DTHF, EPS;
const int THRESHOLD = 0.02;

int main()
{
  const vector<nbody_holder_t>& points = getPoints();
  computeInteractions(points);
  freePoints(points);
  return 0;
}

/*
 * Print out the points for a given vector of either 
 * holders or body objects.
 */
template <class T>
void printPoints(const vector<T>& points)
{
  for (int i = 0; i < points.size(); i++)
    cout << points[i].x << endl;
}

void freePoints(const vector<nbody_holder_t>& pointArray)
{
  for (int i = 0; i < pointArray.size(); i++)
    delete pointArray[i].body;
}

/*
 * Returns a vector of holder structs. This supports good
 * cache performance by only storing the x value in the 
 * holding structure and containing a pointer to the rest.
 */
vector<nbody_holder_t> getPoints()
{
  string cur_line;
  int* outputs[] = { &INPUT_SIZE, &NTIMESTEPS, &DTIME, &EPS };
  for (int i = 0; i < 4; i++) {
    getline(cin, cur_line);
    istringstream s_stream(cur_line);
    s_stream >> *outputs[i];
  }
  DTHF = 0.5 * DTIME;
  getline(cin, cur_line); // ignore TOL
  vector<nbody_holder_t> holder(INPUT_SIZE);
  while (getline(cin, cur_line))
  {
    istringstream s_stream(cur_line);
    s_stream.precision(5);
    nbody_holder_t current;
    nbody_t* body = new nbody_t;
    s_stream >> scientific >> body->mass;
    s_stream >> scientific >> body->x;
    s_stream >> scientific >> body->y;
    s_stream >> scientific >> body->z;
    s_stream >> scientific >> body->vx;
    s_stream >> scientific >> body->vy;
    s_stream >> scientific >> body->vz;
    body->ax = 0;
    body->ay = 0;
    body->az = 0;
    current.x = body->x;
    current.body = body;
    holder.push_back(current);
  }
  return holder;
}

void computeInteractions(const vector<nbody_holder_t>& points)
{
  for (int step = 0; step < NTIMESTEPS; step++) {
    for (int i = 0; i < INPUT_SIZE; i++) {
      for(int j = i+1; j < INPUT_SIZE; j++) {
        points[i].body->ax = points[i].body->ay = points[i].body->az = 0;
        if (abs(points[i].x - points[j].x) < THRESHOLD)
          computeForce(*points[i].body, *points[j].body);
      }
      updatePosition(*points[i].body);
    }
  }
}

// computes the acceleration delta for this particular interaction
void computeForce(nbody_t& first, const nbody_t& second)
{
  double xDist = second.x - first.x;
  double yDist = second.y - first.y;
  double zDist = second.z - first.z;
  
  double rSquared = xDist * xDist + yDist * yDist + zDist * zDist;
  double coefficient = second.mass / pow(rSquared, 1.5);
  
  first.ax += coefficient * xDist;
  first.ay += coefficient * yDist;
  first.az += coefficient * zDist;
}

// computes the final position for a body
void updatePosition(nbody_t& body)
{
  double halfXDelta = body.ax * DTHF;
  double halfYDelta = body.ay * DTHF;
  double halfZDelta = body.az * DTHF;
  
  double meanXVel = body.vx + halfXDelta;
  double meanYVel = body.vy + halfYDelta;
  double meanZVel = body.vz + halfZDelta;
  
  body.x += meanXVel * DTIME;
  body.y += meanYVel * DTIME;
  body.z += meanZVel * DTIME;
  
  body.vx = meanXVel + halfXDelta;
  body.vy = meanYVel + halfYDelta;
  body.vz = meanZVel + halfZDelta;
}