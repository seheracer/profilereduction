#ifndef _MM_H_
#define _MM_H_

#define MM_MAXLINE 1000

struct mmdata {

	int N, M, NNZ;
	int *x;
	int *y;
	double *v;

	int symmetricity;
	int binary;
	int ndiagonal;
	int realnnz;
};

int initialize_mm(char *file, struct mmdata *mm);
int initialize_mm_compact(char *file, struct mmdata *mm);
void printmm(struct mmdata *mm, char *file);
void freemm(struct mmdata *mm);
void reordermm(struct mmdata *mm, int *rowordervec, int *colordervec, struct mmdata *mmordered);

#endif 
