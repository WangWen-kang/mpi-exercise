#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
   int nproc, myid;
   int nmax=2<<20;
   int  nglb, *dspls, *counts, ip, i, bsize;
   double *v_lcl, *v_glb;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

   v_lcl = malloc( nmax * sizeof(double) );
   v_glb = malloc( nmax * sizeof(double) );
   dspls = malloc( (nproc+1)*sizeof(int) );
   counts = malloc( nproc*sizeof(int) );
   if(! (v_lcl && v_glb && dspls && counts )) {
      printf("Error allocating memory.");
      return 1;
   }

/* Define the partition vector dspls(0:np) for a global vector of size nglb
   and the size vector counts(0:np-1) with the sizes of local vectors*/
   dspls[0] = 0;
   for (ip = 1 ; ip<=nproc ; ip++ ) {
      dspls[ip] = dspls[ip-1]+ 3+(ip-1);
      counts[ip-1] = dspls[ip] - dspls[ip-1];
   }
   nglb = dspls[nproc];

/* Initialize the local vectors (such that v_glb(i) = i,i = 0,...,nglb-1) */
   for (i = 0 ; i<counts[myid] ; i++ ) {
         v_lcl[i] = dspls[myid]+i;
   }
   printf("v_lcl on %d >> ",myid);
   for ( i = 0; i < counts[myid]; i++){
      printf("  %2.0f",v_lcl[i]);
   }
   printf("\n");


/* Distribute local vectors v_lcl to their places in v_glb */
/* Copy local vector v_lcl to its places in local copy of v_glb */
   for ( i = 0; i < counts[myid]; i++) {
         v_glb[dspls[myid]+i] = v_lcl[i];
   }
/* Broadcast local parts of v_glb to all processes
     Syntax :
     MPI_Bcast( buffer, count, datatype, root, comm ) 
    .... */

   printf("v_glb on %d >> ",myid);
   for ( i = 0; i < nglb; i++){
      printf("  %2.0f",v_glb[i]); 
   }
   printf("\n");

   MPI_Finalize();
   return 0;
}
