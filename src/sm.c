#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sm.h"

void sm_mm2sm_csr(struct sm_data *s, struct mmdata *m) {

	int i;

	s -> n = m -> N;
	s -> nnz = m -> realnnz;
	s -> xind = (int *)calloc(s -> n + 2, sizeof(int));
	s -> ind = (int *)calloc(s -> nnz, sizeof(int));
	
	for(i=0; i<m -> NNZ; i++) {
		s -> xind[m -> x[i]+2] ++;
		if(m -> symmetricity && m -> x[i] != m -> y[i])
			s -> xind[m -> y[i]+2] ++;
	}
	
	for(i=2; i<=s -> n+1; i++)
		s -> xind[i] += s -> xind[i-1];
		
	for(i=0; i<m -> NNZ; i++) {
		s -> ind[s -> xind[m -> x[i]+1] ++] = m -> y[i];
		if(m -> symmetricity && m -> x[i] != m -> y[i])
			s -> ind[s -> xind[m -> y[i]+1] ++] = m -> x[i];
	}
}

void sm_mm2sm_csc(struct sm_data *s, struct mmdata *m) {

	int i;

	s -> n = m -> M;
	s -> nnz = m -> realnnz;
	s -> xind = (int *)calloc(s -> n + 2, sizeof(int));
	s -> ind = (int *)calloc(s -> nnz, sizeof(int));
	
	for(i=0; i<m -> NNZ; i++) {
		s -> xind[m -> y[i]+2] ++;
		if(m -> symmetricity && m -> x[i] != m -> y[i])
			s -> xind[m -> x[i]+2] ++;
	}
	
	for(i=2; i<=s -> n+1; i++)
		s -> xind[i] += s -> xind[i-1];
		
	for(i=0; i<m -> NNZ; i++) {
		s -> ind[s -> xind[m -> y[i]+1] ++] = m -> x[i];
		if(m -> symmetricity && m -> x[i] != m -> y[i])
			s -> ind[s -> xind[m -> x[i]+1] ++] = m -> y[i];
	}
}

char sm_mm2sm(struct sm_data **p_s, struct mmdata *m, int _type) {

	if(!p_s || !m)
		return -1;	
	
	struct sm_data *s = *p_s = (struct sm_data *)calloc(1, sizeof(struct sm_data));
	s -> type = _type;
	switch(s -> type) {
	
		case SM_TYPE_CSR:	sm_mm2sm_csr(s, m);
					break;
		
		case SM_TYPE_CSC:	sm_mm2sm_csc(s, m);
					break;
	}
	
	return 0;
}

char sm_free(struct sm_data *s) {

	if(!s)
		return -1;
		
	free(s -> xind);
	free(s -> ind);
	free(s);
	
	return 0;
}
