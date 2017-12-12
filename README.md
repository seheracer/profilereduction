moHP: A profile reduction algorithm which is based on hypergraph partitioning.


CONTENTS:

 - src/:		contains source codes (main.c mohp.c pth.c mm.c weightedgreed.c sm.c heap.c stack.c)
   		
 - inc/: 	contains header files (mohp.h pth.h mm.h weightedgreed.h sm.h heap.h stack.h patoh.h)
   		
 - lib/: 	contains PaToH library [1] (libpatoh.a), which is a commonly-used hypergraph partitioner 
   		
 - bin/: 	will contain the executable (mohp) after compiling 
   		
 - test/: 	contains a sparse matrix instance (nasa1824) obtained from SuiteSparse Matrix Collection [2] and a script for running mohp on that instance.

 - makefile

COMPILE:

 - simply type 'make' in this directory

RUN:
 
 - './run.sh' runs the executable mohp (located in bin/ directory) on nasa1824.mtx instance with default settings
   default settings refer to using loadimbalance=0.90, threshold=0.25, and outputfile='permuted.mtx' (the permuted matrix will be written in 'permuted.mtx')
   
 - in order to run mohp with a different setting, use command 'executable matrixfile -i loadimbalance -t threshold -o outputfile' 
 
NOTES

- matrixfile is assumed to be a matrix market file, can be obtained from SuiteSparse Matrix Collection [2] 

REFERENCES

[1] http://bmi.osu.edu/~umit/software.html
[2] https://sparse.tamu.edu