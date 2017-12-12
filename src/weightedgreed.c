#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weightedgreed.h"

int max(int x, int y) {

  return x>y? x: y;
}

int hweight(int x) {

  if(x > THRESHOLD) {
	
    return CONSTANT / THRESHOLD;
  }
  if(x == 0)
    return 2*CONSTANT;
  else
    return CONSTANT / x;
}

int read_stages(int *partvec, int n, int *stages) {

  int i;
  int nstages = 0;

  for(i=0; i<n; i++) {
    stages[i] = partvec[i];
    if(stages[i] + 1 > nstages)
      nstages = stages[i] + 1;
  }

  return nstages;
}

void orderstages(int *stages, int n, int *xstages, int *ostages, int nstages)
{
  int i;
  for(i = 0; i < n; i++)
    xstages[stages[i]+2]++;

  for(i = 2; i < nstages+2; i++)
    xstages[i] += xstages[i-1];

  for(i = 0; i < n; i++)
    ostages[xstages[stages[i]+1] ++] = i;

}


int modifiedweightedgreed(struct mmdata *m, int *partvec, int *perm)
{
  int i,j,k,s,t,r,l,c,p,nstages,*iperm;
  iperm = (int *)calloc(m->N, sizeof(int));
  
  struct sm_data *r2c, *c2r;
  sm_mm2sm(&r2c, m, SM_TYPE_CSR);
  sm_mm2sm(&c2r, m, SM_TYPE_CSC);

  int *stages = (int *)calloc(m->N, sizeof(int));
  nstages = read_stages(partvec, m->N, stages);
  
  int *xstages = (int *)calloc(nstages+2, sizeof(int));
  int *ostages = (int *)malloc(m->N*sizeof(int));

  orderstages(stages, m->N, xstages, ostages, nstages);


  int *udegrees = (int *)calloc(m->N, sizeof(int));
  for(i=0; i<m->N; i++)
    udegrees[i] = r2c->xind[i+1] - r2c->xind[i];
	
  int *vals = (int *)calloc(m->N, sizeof(int));
  int *fvals = (int *)calloc(m->N, sizeof(int));
  for(i=0; i<m->N; i++) {
    vals[i] = max(hweight(udegrees[i]), hweight(m->N-udegrees[i]+1));
    fvals[i] = vals[i];
  }

  int niperm = m->N;
  struct heap_data *h= (struct heap_data *)calloc(sizeof(struct heap_data),1);
  heap_init(h, m->N, m->N, HEAP_MAX);

  struct heap_data *hd= (struct heap_data *)calloc(sizeof(struct heap_data),1);
  heap_init(hd, m->N, m->N, HEAP_MAX);

  struct stack_data *st = (struct stack_data *) calloc(sizeof(struct stack_data), 1);
  stack_init(st, 2*m->N);
	
  int oldfval,wd,nrs;
  int *rs = (int *)calloc(m->N, sizeof(int));

  for(k=m->N,s=nstages-1; s>=0; s--) {
		
    for(p=xstages[s+1]-1; p >=xstages[s]; p--) {
      j = ostages[p];
      heap_insert(hd, j, udegrees[j]);
      
      oldfval = fvals[j];
      fvals[j] = max(hweight(udegrees[j]), hweight(k-udegrees[j]+1));
      wd = oldfval - fvals[j];
      vals[j] -= wd;
      
      heap_insert(h, j, vals[j]);
    }

    while(h->size) {

      j = heap_extractopt(h);
      iperm[--niperm] = j;
      stages[j] = -1;
      k --;

      for(t=c2r->xind[j]; t<c2r->xind[j+1]; t++) {
        r = c2r->ind[t];
        udegrees[r] --;
        if(stages[r] == -1) {

          if(udegrees[r] < THRESHOLD && udegrees[r] > 0) {
            wd = hweight(udegrees[r]+1) - hweight(udegrees[r]);
            for(l=r2c->xind[r]; l<r2c->xind[r+1]; l++) {
						
              c = r2c->ind[l];
              if(stages[c] != -1) {
                vals[c] -= wd;							
                if(stages[c] == s)
                  heap_update(h, c, vals[c]);
              }
							
            }
          }
        }else {
				
          if(stages[r] == s)
            heap_update(hd, r, udegrees[r]);
        }
      }
			

      stack_push(st, 0);
      while(st->size) {
			
        i = stack_pop(st);
        if(i >= hd->size)
          continue;

					
        r = hd->items[i]; 
        if(k - udegrees[r] + 1 >= THRESHOLD)
          continue;
					
        oldfval = fvals[r];
        fvals[r] = max(hweight(udegrees[r]), hweight(k-udegrees[r]+1));
        wd = oldfval - fvals[r];
        if(wd) {
          vals[r] -= wd;
          if(stages[r] == s)
            heap_update(h, r, vals[r]);
        }
				
        stack_push(st, 2*i+1);
        stack_push(st, 2*i+2);
      }

      for(t=c2r->xind[j]; t<c2r->xind[j+1]; t++) {
		
        r = c2r->ind[t];
        if(stages[r] != -1) {
				
          oldfval = fvals[r];
          fvals[r] = max(hweight(udegrees[r]), hweight(k-udegrees[r]+1));
					
          wd = oldfval - fvals[r];
          if(wd) {
					
            vals[r] -= wd;
            if(stages[r] == s)
              heap_update(h, r, vals[r]);
          }
        }
      }
					
    }
  }

  for(i = 0; i < m->N; i++)
    perm[iperm[i]] = i;
  
  sm_free(r2c);
  sm_free(c2r);

  free(iperm);
  free(stages);
  free(xstages);
  free(ostages);
  free(udegrees);
  free(vals);
  free(fvals);
  free(rs);
	
  return 0; 
}
