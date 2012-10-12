#include "nbody.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

static int INPUT_SIZE, NTIMESTEPS, THRESHOLD;
int main()
{
  const vector<nbody_holder_t>& points = getPoints();
  computeCollisions(points);
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
  for (int i = 0; i < 2; i++) {
    getline(cin, cur_line);
    istringstream s_stream(cur_line);
    if (i)
      s_stream >> NTIMESTEPS;
    else s_stream >> INPUT_SIZE;
  }
  for (int i = 2; i <= 5; i++)
    getline(cin, cur_line);
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
    current.x = body->x;
    current.body = body;
    holder.push_back(current);
  }
  return holder;
}

template <class T>
void computeCollisions(const vector<T>& points)
{
  for (int step = 0; step < NTIMESTEPS; step++) {
    for (int i = 0; i < INPUT_SIZE; i++) {
      for(int j = i+1; j < INPUT_SIZE; j++) {
        if (abs(points[i].x - points[j].x) < 0.02)
          computeNewVelocities(points[i], points[j]);
      }
    }
  }
}

template <class T>
// Something here to compute new velocities of the points
void computeNewVelocities(const T& first, const T& second)
{

}
