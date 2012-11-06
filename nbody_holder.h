#ifndef NBODY_HOLDER_H
#define NBODY_HOLDER_H

#include "nbody.h"

typedef struct _nbody_holder_t {
  double x;
  nbody_t* body;
} nbody_holder_t;

void freePoints(nbody_holder_t* points);
int compareHolders(const void* holderA, const void* holderB);
/* Function declarations */
nbody_holder_t* getHolders();
void printHolders(const nbody_holder_t* points);
void computeHolderInteractions(nbody_holder_t* points);
void computeHolderSortedInteractions(nbody_holder_t* points);
void computeHolderBlockedInteractions(nbody_holder_t* points);

#endif

