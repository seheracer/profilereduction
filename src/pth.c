#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pth.h"

void pthextend(struct pthdata *opth, struct pthdata *pth)
{
  int i, j, c;
	
  pth->c = opth->c+2;
  pth->n = opth->n *2;
  pth->p = opth->p+3*opth->n;
  pth->nconst = 1;
	
  pth->cwghts = (int*)calloc(pth->c, sizeof(int));
  pth->nwghts = (int*)calloc(pth->n, sizeof(int));
  pth->xpins = (int*)calloc(pth->n+2, sizeof(int));
  pth->pins = (int*)calloc(pth->p, sizeof(int));
  pth->nstates = (int*)calloc(pth->n, sizeof(int));
  pth->nlocalnodes = (int*)calloc(pth->n, sizeof(int));
  pth->n = pth->p = 0;
	
  for(i = 0;i < pth->c-2; i++)
    pth->cwghts[i] = opth->cwghts[i];
  pth->cwghts[pth->c-2] = pth->cwghts[pth->c-1] = 0;
	
  pth->xpins[0] = 0;	
  for(i = j = 0;i < opth->n; i++)
    {
      for( ; j < opth->xpins[i+1]; j++)
	{
	  c = opth->pins[j];
	  pth->pins[pth->p++] = c;
	}
		
      pth->pins[pth->p++] = pth->c-1;
      pth->xpins[pth->n+1] = pth->p;
      pth->nwghts[pth->n] = 1;
      pth->nstates[pth->n] = PTH_NETSTATE_ORIG_PAIR;
      pth->nlocalnodes[pth->n] = i;
      pth->n++;
		
      pth->pins[pth->p++] = i;
      pth->pins[pth->p++] = pth->c-2;
      pth->xpins[pth->n+1] = pth->p;
      pth->nwghts[pth->n] = 1;;
      pth->nstates[pth->n] = PTH_NETSTATE_PRIME_PAIR;
      pth->nlocalnodes[pth->n] = i;
      pth->n++;
  }

}

void mm2pth(struct mmdata *mm, struct pthdata *pth) {

  int i, j, *ins;
	
  pth->c = mm->M;
  pth->n = mm->N;
  pth->p = mm->realnnz;
  pth->nconst = 1;
	
  pth->xpins = (int *)calloc(pth->n+2, sizeof(int));
  pth->cwghts = (int *)calloc(pth->c, sizeof(int));
  pth->nwghts = (int *)calloc(pth->n, sizeof(int));
	
  ins = (int *)calloc(mm->N, sizeof(int));
  for(i=0; i<mm->NNZ; i++)
    {
      pth->xpins[mm->x[i]+2] ++;
      if(mm->symmetricity && mm->x[i] != mm->y[i])
	pth->xpins[mm->y[i]+2] ++;
      if(mm->x[i] == mm->y[i])
	ins[mm->x[i]] = 1;
    }

  for(i=0; i < pth->n; i++) 
    if(ins[i] == 0)
      {
	pth->xpins[i+2]++;
	pth->p++;
      }

  for(i=2; i<=pth->n; i++) 
    pth->xpins[i] += pth->xpins[i-1];

  pth->pins = (int *)malloc(sizeof(int) * pth->p);
  for(i=0; i<mm->NNZ; i++)
    {
      pth->pins[ pth->xpins[ mm->x[i]+1 ] ++ ] = mm->y[i];

      if(mm->symmetricity && mm->x[i] != mm->y[i])
	pth->pins[ pth->xpins[mm->y[i]+1] ++ ] = mm->x[i];
  }

  for(i=0; i<pth->n; i++)
    if(ins[i] == 0)
      pth->pins[pth->xpins[i+1]++ ] = i;

  for(i=0; i<pth->n; i++) 
    pth->nwghts[i] = 1;
	
  for(i=0; i<pth->c; i++) 
    pth->cwghts[i] = 1;

  free(ins);
}


void pthpartition(struct pthdata *pth, int kway, double loadimbalance, int *partvec) {

  int cutsize;
  int *partweights = (int *)malloc(sizeof(int) * kway);

  PaToH_Parameters args;
  PaToH_Initialize_Parameters(&args, PATOH_CUTPART, PATOH_SUGPARAM_DEFAULT);
  args._k = kway;
  args.final_imbal = loadimbalance;
  args.crs_alg = 6;
  args.MemMul_Pins = 256;
  args.MemMul_CellNet = 256;
  args.crs_VisitOrder = 6;
  args.crs_coarsento = 25;
  args.nofinstances = 2;
  args.initp_runno = 1;
  args.ref_alg = 6;
  args.seed = 1;

  PaToH_Alloc(&args, pth->c, pth->n, pth->nconst, pth->cwghts, pth->nwghts, pth->xpins, pth->pins);

  float tw[2] = {0.5, 0.5};

  PaToH_Part(&args, pth->c, pth->n, pth->nconst, 1, pth->cwghts, pth->nwghts, pth->xpins, pth->pins, tw, partvec, partweights, &cutsize);
  PaToH_Free();

  free(partweights);
}


void printpth(struct pthdata *pth) {

  printf("number of cells: %d\n", pth->c);
  printf("number of nets: %d\n", pth->n);
  printf("number of pins: %d\n", pth->p);

  int i;
  printf("xpins: ");
  for(i=0; i<=pth->n; i++)
    printf("%d ", pth->xpins[i]);
  printf("\n");

  printf("pins: ");
  for(i=0; i<pth->p; i++)


    printf("%d ", pth->pins[i]);
  printf("\n");
}

void freepth(struct pthdata *pth) {

  free(pth->cwghts);
  free(pth->nwghts);
  free(pth->xpins);
  free(pth->pins);
  free(pth);
}

void freepthe(struct pthdata *pth) {

  free(pth->cwghts);
  free(pth->nwghts);
  free(pth->xpins);
  free(pth->pins);
  free(pth->nstates);
  free(pth->nlocalnodes);
  free(pth);
}
