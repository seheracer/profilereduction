#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mohp.h"


void split_orig_pair(struct pthdata *pth, int *lpartvec, struct pthdata *pthleft, struct pthdata *pthright, int *map, int n) {

  int j, v, c;	
  v = pth->nlocalnodes[n];	
  if(lpartvec[v] == 0)  // v_i is in the left part 
    {		
      for(j=pth->xpins[n]; j<pth->xpins[n+1]; j++)
	{ 	
	  c = pth->pins[j];
	  if(c < pth->c-2 && lpartvec[c] == 0)
	    pthleft->pins[pthleft->p++] = map[c];
	}
	
      pthleft->pins[pthleft->p++] = pthleft->c-1;	
      pthleft->xpins[pthleft->n+1] = pthleft->p;	
      pthleft->nwghts[pthleft->n] = 1;
      pthleft->nstates[pthleft->n] = PTH_NETSTATE_ORIG_PAIR;
      pthleft->nlocalnodes[pthleft->n] = map[v];
      pthleft->n++;
		
      pthleft->pins[pthleft->p++] = pthleft->c-2;
      pthleft->pins[pthleft->p++] = map[v];
      pthleft->xpins[pthleft->n+1] = pthleft->p;
      pthleft->nwghts[pthleft->n] = pthleft->nwghts[pthleft->n-1];
      pthleft->nstates[pthleft->n] = PTH_NETSTATE_PRIME_PAIR;
      pthleft->nlocalnodes[pthleft->n] = map[v];
      pthleft->n++;
    }
  else 		      // v_i is in the right part		
    {	
      for(j=pth->xpins[n]; j<pth->xpins[n+1]; j++)
	{
		
	  c = pth->pins[j];
	  if(c < pth->c-2) {
	    if(lpartvec[c] == 0)
	      pthleft->pins[pthleft->p++] = map[c];
	    else 
	      pthright->pins[pthright->p++] = map[c];
	  }
	}
    
      if(pthleft -> xpins[pthleft ->n] < pthleft -> p) // it has at last one pin in the left part: left-cut.
	{
	  pthleft->pins[pthleft->p++] = pthleft->c-1;
	  pthleft->xpins[pthleft->n+1] = pthleft->p;
	  pthleft->nwghts[pthleft->n] = 1;
	  pthleft->nstates[pthleft->n] = PTH_NETSTATE_ORIG_SINGLE;
	  pthleft->nlocalnodes[pthleft->n] = -1;
	  pthleft->n++;
			
	  pthright->p = pthright->xpins[pthright->n];
	  pthright->pins[pthright->p++] = pthright->c-2;
	  pthright->pins[pthright->p++] = map[v];
	  pthright->xpins[pthright->n+1] = pthright->p;
	  pthright->nwghts[pthright->n] = 1;
	  pthright->nstates[pthright->n] = PTH_NETSTATE_PRIME_SINGLE;
	  pthright->nlocalnodes[pthright->n] = map[v];
	  pthright->n++;
	}
      else                               // it is internal to the right part
	{
		
	  pthleft->p = pthleft->xpins[pthleft->n];
			
	  pthright->pins[pthright->p++] = pthright->c-1;
	  pthright->xpins[pthright->n+1] = pthright->p;
	  pthright->nwghts[pthright->n] = 1;
	  pthright->nstates[pthright->n] = PTH_NETSTATE_ORIG_PAIR;
	  pthright->nlocalnodes[pthright->n] = map[v];
	  pthright->n++;
			
	  pthright->pins[pthright->p++] = pthright->c-2;
	  pthright->pins[pthright->p++] = map[v];
	  pthright->xpins[pthright->n+1] = pthright->p;
	  pthright->nwghts[pthright->n] = pthright->nwghts[pthright->n-1];
	  pthright->nstates[pthright->n] = PTH_NETSTATE_PRIME_PAIR;
	  pthright->nlocalnodes[pthright->n] = map[v];
	  pthright->n++;
	}
    }
}

void split_orig_single(struct pthdata *pth, int *lpartvec, struct pthdata *pthleft, struct pthdata *pthright, int *map, int n) {

  int j, c;		
  for(j = pth->xpins[n]; j < pth->xpins[n+1]; j++)
    {		
      c = pth->pins[j];
      if(c < pth->c-2)
	{
	  if(lpartvec[c] == 0)
	    pthleft->pins[pthleft->p++] = map[c];
	  else
	    pthright->pins[pthright->p++] = map[c];
     
	}
    }
	
  // if net has pins in the left part	
  if(pthleft->xpins[pthleft->n] < pthleft->p)
    {
      pthleft->pins[pthleft->p++] = pthleft->c-1;
      pthleft->xpins[pthleft->n+1] = pthleft->p;
      pthleft->nwghts[pthleft->n] = 1;
      pthleft->nstates[pthleft->n] = PTH_NETSTATE_ORIG_SINGLE;
      pthleft->nlocalnodes[pthleft->n] = -1;
      pthleft->n++;
		
      pthright->p = pthright->xpins[pthright->n];
    }
  else
    {
      pthright->pins[pthright->p++] = pthright->c-1;
      pthright->xpins[pthright->n+1] = pthright->p;
      pthright->nwghts[pthright->n] = 1;
      pthright->nstates[pthright->n] = PTH_NETSTATE_ORIG_SINGLE;
      pthright->nlocalnodes[pthright->n] = -1;
      pthright->n++;
    }
}


void split_prime_single(struct pthdata *pth, int *lpartvec, struct pthdata *pthleft, struct pthdata *pthright, int *map, int n) {

  int v = pth->nlocalnodes[n];	
	
  if(lpartvec[v] == 0)
    {
      pthleft->pins[pthleft->p++] = pthleft->c-2;
      pthleft->pins[pthleft->p++] = map[v];
      pthleft->xpins[pthleft->n+1] = pthleft->p;
      pthleft->nwghts[pthleft->n] = 1;
      pthleft->nstates[pthleft->n] = PTH_NETSTATE_PRIME_SINGLE;
      pthleft->nlocalnodes[pthleft->n] = map[v];
      pthleft->n++;
    }
  else
    {
      pthright->pins[pthright->p++] = pthright->c-2;
      pthright->pins[pthright->p++] = map[v];
      pthright->xpins[pthright->n+1] = pthright->p;
      pthright->nwghts[pthright->n] = 1;
      pthright->nstates[pthright->n] = PTH_NETSTATE_PRIME_SINGLE;
      pthright->nlocalnodes[pthright->n] = map[v];
      pthright->n++;
    }
	
}


void form(struct pthdata *pth, struct pthdata *pthleft, struct pthdata *pthright, int *lpartvec, int *map) {
	
  int i, j, left, right, c, en, *cw;

  pthleft->c = pthright->c = 0;
  pthleft->n = pthright->n = 0;
  pthleft->p = pthright->p = 0;
  pthleft->nconst = pthright->nconst = 1;
	
  pthleft->xpins = (int *)malloc(sizeof(int) * (pth->n+1));
  pthright->xpins = (int *)malloc(sizeof(int) * (pth->n+1));
  pthleft->nwghts = (int *)malloc(sizeof(int) * pth->n);
  pthright->nwghts = (int *)malloc(sizeof(int) * pth->n);
  pthleft->nstates = (int *)malloc(sizeof(int) * pth->n);
  pthright->nstates = (int *)malloc(sizeof(int) * pth->n);
  pthleft->nlocalnodes = (int *)malloc(sizeof(int) * pth->n);
  pthright->nlocalnodes = (int *)malloc(sizeof(int) * pth->n);
  pthleft->pins = (int *)malloc(sizeof(int) * pth->p);
  pthright->pins = (int *)malloc(sizeof(int) * pth->p);

  for(i=0; i<pth->c-2; i++)
    if(lpartvec[i] == 0)
      map[i] = pthleft->c ++;
    else
      map[i] = pthright->c ++;

  pthleft->c += 2;
  pthright->c += 2;
	
  pthleft->cwghts = (int *)malloc(sizeof(int) * pthleft->c);
  pthright->cwghts = (int *)malloc(sizeof(int) * pthright->c);
  
  en = pthleft->c-2;
  cw = pthleft->cwghts;
  for(i = 0; i < en; i++)
    cw[i] = 1;
  
  cw[en++] = 0;
  cw[en++] = 0;

  en = pthright->c-2;
  cw = pthright->cwghts;
  for(i = 0; i < en; i++)
    cw[i] = 1;
  
  cw[en++] = 0;
  cw[en++] = 0;
 
  
  pthleft->xpins[0] = 0;
  pthright->xpins[0] = 0;

  for(i=0; i<pth->n; i++) {
    switch(pth->nstates[i]) {
		
    case PTH_NETSTATE_ORIG_PAIR:	split_orig_pair(pth, lpartvec, pthleft, pthright, map, i);
      break;
			
    case PTH_NETSTATE_PRIME_PAIR:	//do nothing, all prime pair nets are handled in split_orig_pair function 
      break;

    case PTH_NETSTATE_ORIG_SINGLE:	split_orig_single(pth, lpartvec, pthleft, pthright, map, i);
      break;
			
    case PTH_NETSTATE_PRIME_SINGLE:	split_prime_single(pth, lpartvec, pthleft, pthright, map, i);
      break;
			
    }
  }

}

int mohp_rec(struct pthdata *pth, double loadimbalance, int threshold, int *part)
{
  if(pth->c-2 <= threshold)
    return 1;
  else
    {
      int i, lcnt, rcnt, *lpartvec, *partleft, *partright, *map;
      struct pthdata *pthleft, *pthright;
      
      lpartvec = (int *)malloc(pth->c*sizeof(int));
      for(i = 0; i < pth->c-2; i++)
	lpartvec[i] = -1;	
      lpartvec[pth->c-2] = 0;
      lpartvec[pth->c-1] = 1;

      //bipartition
      pthpartition(pth, 2, loadimbalance, lpartvec);

      //form left and right hypergraphs
      pthleft = (struct pthdata *)calloc(1, sizeof(struct pthdata));
      pthright = (struct pthdata *)calloc(1, sizeof(struct pthdata));
      map = (int *)malloc(pth->c*sizeof(int));
      form(pth, pthleft, pthright, lpartvec, map);

      //allocate part vectors for left and right hypergraph
      partleft = (int *)calloc(pthleft->c, sizeof(int));
      partright = (int *)calloc(pthright->c, sizeof(int));
      
      //recursive mohp_rec invocation on the left hypergraph
      lcnt = mohp_rec(pthleft, loadimbalance, threshold, partleft);
      for(i = 0; i < pth->c-2; i++)
	if(lpartvec[i] == 0)
	  part[i] = partleft[map[i]];
      
      free(partleft);
      freepthe(pthleft);

      //recursive mohp_rec invocation on the right hypergraph
      rcnt = mohp_rec(pthright, loadimbalance, threshold, partright);
      for(i = 0; i < pth->c-2; i++)
	if(lpartvec[i] == 1)
	  part[i] = lcnt + partright[map[i]];
      
      free(partright);
      freepthe(pthright);

      free(lpartvec);
      free(map);
      
      return lcnt + rcnt;
    }

}

void part2perm(int *part, int *perm, int n) {

  int i, *sizes, *iperm;
	
  sizes = (int *)calloc(n+2, sizeof(int));

  for(i = 0; i < n; i++) 
    sizes[part[i]+2] ++;	
	
  for(i = 2; i < n+2; i++)
    sizes[i] += sizes[i-1];

  iperm = (int *)calloc(n, sizeof(int));
  for(i = 0; i < n; i++)
    iperm[sizes[part[i]+1] ++] = i;	

  for(i = 0; i < n; i++)
    perm[iperm[i]] = i;
  
  free(iperm);
  free(sizes);
}

void mohp(struct mmdata *mm, double loadimbalance, int threshold, int *perm, double *eltime)
{
  int *part;
  struct pthdata *opth, *pth;
  clock_t start, end;

  //form initial row-net hypergraph
  opth = (struct pthdata *) malloc(sizeof(struct pthdata));
  mm2pth(mm, opth);
	
  //extend the initial hypergraph
  pth = (struct pthdata *) malloc(sizeof(struct pthdata));
  pthextend(opth, pth);
  freepth(opth);
  
  //initialize ordered partition 
  part = (int *)calloc(pth->c, sizeof(int));

  start = clock();
  //recursive call to the mohp function
  if(pth->c-2 > threshold)
    mohp_rec(pth, loadimbalance, threshold, part);
  
  //convert the ordered partition to a permutation
  if(threshold > 1)
    modifiedweightedgreed(mm, part, perm);
  else
    part2perm(part, perm, mm->N);
    
  end = clock();
  *eltime = (double)(end - start)/CLOCKS_PER_SEC;
  
  free(part);
  freepthe(pth);

}

