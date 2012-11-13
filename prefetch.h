#ifndef NBODY_PREFETCH_H
#define NBODY_PREFETCH_H

#include "nbody.h"
#include "nbody_holder.h"

void computePrefetchInteractions(nbody_t* points);
void computeBlockedPrefetchInteractions(nbody_t* points);
void computeSortedPrefetchInteractions(nbody_t* points);
void computeHolderPrefetchInteractions(nbody_holder_t* points);
void computeHolderBlockedPrefetchInteractions(nbody_holder_t* points);
void computeHolderSortedPrefetchInteractions(nbody_holder_t* points);

#endif