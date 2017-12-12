#ifndef _PTH_H_ 
#define _PTH_H_

#include "mm.h"
#include "patoh.h"

#define PTH_NETSTATE_ORIG_PAIR	        0
#define PTH_NETSTATE_PRIME_PAIR	        1
#define PTH_NETSTATE_PRIME_SINGLE	2
#define PTH_NETSTATE_ORIG_SINGLE	3

struct pthdata {

	int c;		//number of vertices
	int n;		//number of nets
	int p;		//number of pins;
	int nconst;	//number of constraints

	int *cwghts;	//vertex weights
	int *nwghts;	//net costs
	int *xpins;	
	int *pins;	

	int *nstates;	
	int *nlocalnodes;
};

void mm2pth(struct mmdata *mm, struct pthdata *pth);
void printpth(struct pthdata *pth);
void freepth(struct pthdata *pth);
void freepthe(struct pthdata *pth);
void pthextend(struct pthdata *opth, struct pthdata *pth);
void pthpartition(struct pthdata *pth, int kway, double loadimbalance, int *partvec);

#endif
