#ifndef _SM_H_
#define _SM_H_

#include "mm.h"

#define SM_TYPE_CSR	0
#define SM_TYPE_CSC	1

struct sm_data {

	int n;
	int nnz;
	int *xind;
	int *ind;
	
	int type;
};

char sm_mm2sm(struct sm_data **p_s, struct mmdata *m, int _type);

#endif 
