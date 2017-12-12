#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <math.h>

#include "mm.h"
#include "pth.h"
#include "mohp.h"

#define LSTRING 1024

char matrixfile[LSTRING];
char outfile[LSTRING];
double loadimbalance;
int threshold;

void printusage()
{
  printf("usage: mohp [options] matrixfile\n");
  printf("\t-t threshold: (int) the part size for terminating recursion [25]\n");
  printf("\t-i loadimbalance: (double) load imbalance for bipartitions [0.90] \n");
  printf("\t-o outputfile: (string) output file where the permuted matrix is to be written\n");
  exit(1);
}

void getdefaults()
{
  threshold = 25;
  loadimbalance = 0.90;
  strcpy(outfile, "permuted.mtx");
}


void parseinputs(int argc, char *argv[])
{
  int c;
  while ((c = getopt(argc, argv, "t:i:o:")) != -1)
    {
      switch (c)
	{
        case 't': 	threshold = atoi(optarg);
          break;

        case 'i': 	loadimbalance = atof(optarg);
          break;

        case 'o':	sprintf(outfile, "%s", optarg);
          break;
	}
    }

  if (argc <= optind)
    printusage();

  strcpy(matrixfile, argv[optind]);
}

void initialize_parameters(int argc, char *argv[])
{
  getdefaults();
  parseinputs(argc, argv);
}


long long compute_profile(struct mmdata *mm, int *perm)
{
  int i, r, c, *firsts;
  long long profile;
  
  firsts = (int *)malloc(mm->N*sizeof(int));
	
  for(i=0; i<mm->N; i++)
    firsts[i] = i;

  for(i=0; i<mm->NNZ; i++)
    {
      r = perm[mm->x[i]];
      c = perm[mm->y[i]];

      if(c < firsts[r])
	firsts[r] = c;
			
      if(mm->symmetricity)
	{
	  r = perm[mm->y[i]];
	  c = perm[mm->x[i]];
	  if(c < firsts[r])
	    firsts[r] = c;
	}
    }			
  
  profile = 0;
  for(i = 0; i < mm->N; i++)
    profile += i - firsts[i];

  free(firsts);

  return profile;
}



void writepermutedmatrix(struct mmdata *mm, int *perm, char out[])
{
  FILE *f = fopen(out, "w");
  fprintf(f, "%c%cMatrixMarket matrix coordinate %s general\n", '%', '%', mm->binary? "pattern": "real");
  fprintf(f, "%d %d %d\n", mm->N, mm->M, mm->realnnz);

  int i;
  if(mm->binary)
    for(i=0; i<mm->NNZ; i++)
      {
	fprintf(f, "%d %d\n", perm[mm->x[i]]+1, perm[mm->y[i]]+1);
	if(mm->symmetricity && mm->x[i] != mm->y[i])
	  fprintf(f, "%d %d\n", perm[mm->y[i]]+1, perm[mm->x[i]]+1);
      }
  else
    for(i=0; i<mm->NNZ; i++)
      {
	fprintf(f, "%d %d %f\n", perm[mm->x[i]]+1, perm[mm->y[i]]+1, mm->v[i]);
	if(mm->symmetricity && mm->x[i] != mm->y[i])
	  fprintf(f, "%d %d %f\n", perm[mm->y[i]]+1, perm[mm->x[i]]+1, mm->v[i]);
      }
    
  fclose(f);

}


int main(int argc, char *argv[])
{
  int i, *initialperm, *resultperm;
  long long initialprofile, resultprofile;
  double eltime;
  struct mmdata *mm;
  
  initialize_parameters(argc, argv);
  
  //read .mtx file into mmdata struct
  mm = (struct mmdata *) malloc(sizeof(struct mmdata));
  initialize_mm(matrixfile, mm);
  
  initialperm = (int *)malloc(sizeof(int)*mm->N);
  for(i = 0; i < mm->N; i++)
    initialperm[i]=i;

  //compute initial profile
  initialprofile = compute_profile(mm, initialperm);
  printf("\n======== matrix properties ===========\nname:\t\t\t%s\n#rows/columns:\t\t%d\n#nonzeros:\t\t%d\ninitial profile:\t%lld\n", matrixfile, mm->N, mm->realnnz, initialprofile);

  printf("\n======== parameters for moHP =========\nload imbalance:\t\t%.2f\nthreshold:\t\t%d\n", loadimbalance, threshold );

  //run mohp algorithm
  resultperm = (int *)calloc(mm->N,sizeof(int));
  mohp(mm, loadimbalance, threshold, resultperm, &eltime);

  //compute resulting profile 
  resultprofile = (double) compute_profile(mm, resultperm);

  printf("\n======== result of moHP =========\nresulting profile:\t%lld\nprofile reduction rate:\t%.1f\%\nelapsed time:\t\t%.2f seconds\n", resultprofile,((double)(initialprofile-resultprofile)/initialprofile)*100, eltime);

  //write the permuted matrix to the specified file
  writepermutedmatrix(mm, resultperm, outfile);

  printf("\npermuted matrix is written in %s\n\n", outfile);

  free(resultperm);
  free(initialperm);
  freemm(mm);

  return 0;
}
