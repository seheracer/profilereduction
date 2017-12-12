#ifndef _WEIGHTEDGREED_
#define _WEIGHTEDGREED_ 

#include "sm.h"
#include "heap.h"
#include "stack.h"

#define CONSTANT 840
#define THRESHOLD 8

int modifiedweightedgreed(struct mmdata *m, int *partvec, int *perm);

#endif 
