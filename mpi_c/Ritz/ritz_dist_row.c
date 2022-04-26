
/*
  +-------------------------------------------------------------------+
  |                                                                   |
  |  This program determines the largest eigenvalue of a matrix using |
  |  the method of Raleigh and Ritz.                                  |
  |                                                                   |
  |   Distributed version 2 - row blocks                              |
  +-------------------------------------------------------------------+
*/

#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ritz.h"

int main(int argc,char **argv)
{ 
   int nproc, myid;
   int nmax= 8100;
   int i, j, n, nl, il, nloop=100, istop, firstind[NPROCMAX];
   double *a, *al, *x, *y; 
   double flops, tcpu, evn, evo, res;
   double t1, t2, t3, t4, t5, tmv=0.0, tcl=0.0, tte=0.0, tsc=0.0;

   /* Allocate memory */
   a = (double*) malloc(nmax*nmax*sizeof(double));
   al = (double*) malloc(nmax*nmax*sizeof(double));
   x = (double*) malloc(nmax*sizeof(double));
   y = (double*) malloc(nmax*sizeof(double));

   if(! (a && al && x && y) ) {
      printf("Error allocating memory.\n");
      return 1;
   }

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

   /* Initialization on proc 0; */
   if (myid == 0){

      /* get matrix size */
      printf("\n  Raleigh-Ritz running ... \n  N = ? \n");
      scanf("%d",&n);
      if  (n <= 0){
         printf("%d : N le 0\n",n);
         return -1;
      }
      if  (n > nmax){
         printf("%d : N gt nmax \n",n);
         return -1;
      }
      /* initialize global matrix */
      printf(" \n Initializing data ...\n");
      for (j = 1; j <= n;j++){
         for (i = 1; i < j;i++){
            a[i-1 + n * (j - 1)] = a[j-1 + n * (i - 1)];
         }
         for (i = j; i <= n;i++){
            a[i-1 + n * (j - 1)] = ( (double)i + (double)j ) / (double)i;
         }
      }
   }
   MPI_Barrier( MPI_COMM_WORLD );

   /* distribute global matrix */
   MPI_Bcast( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );
/*      printf("myid: %d \n",myid);
      for (i = 1; i <= n;i++){
         printf("%f %f %f %f \n",a[i-1],a[i-1+n],a[i-1+2*n],a[i-1+3*n]);
         }*/
   dist_index( n, firstind );
   nl = firstind[myid+1] - firstind[myid];
   dist_matrix_rowblock( n, n, a, al );

   MPI_Barrier( MPI_COMM_WORLD );
/*      for (i = 1; i <= nl;i++){
               printf("%d : %f %f %f %f \n",myid, al[i-1],al[i-1+nl],al[i-1+2*nl],al[i-1+3*nl]);
                       } */
/*
  --------------------------------------------------------------
  Raleigh-Ritz algoritm
  row blocks version
  --------------------------------------------------------------
*/
   /* initialize start vector x */
   for (i = 1; i <= nl;i++) 
      y[i-1] = 1.0;

   tcpu = MPI_Wtime();
  
   evo = y[0];
   istop = 0;
   for (il = 1; il <= nloop;il++){
      t1 = MPI_Wtime();

      /* Gather global vector */
      global_vector(n,y,x);
 
      t2 = MPI_Wtime();
      /* Calculate y = al * x  */
      mv(nl, n, al, x, y);

      t3 = MPI_Wtime();

      if (myid == 0){
         /* Get eigenvector, calculate diff. to last iteration */
         evn = y[0];
         res = (evn - evo) / evn;
//         printf("%d %20.15f %e \n",il,evn,res);
         if (fabs(res) < 1.0e-12) istop = 1;
      }

      MPI_Bcast( &evn, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
      MPI_Bcast( &istop, 1, MPI_INT, 0, MPI_COMM_WORLD );
      evo = evn;

      t4 = MPI_Wtime();
      if (istop == 1) break;
      for (i = 1; i <= nl;i++){
         y[i-1] = y[i-1] / evn;
      }

      t5 = MPI_Wtime();
      tmv = tmv + t3 - t2;
      tcl = tcl + t2 - t1;
      tte = tte + t4 - t3;
      tsc = tsc + t5 - t4;
   }

   tcpu = MPI_Wtime() - tcpu;

   /* print results */
   if (myid == 0){
      if (tcpu > 0.0)
         flops = (double) (2 * il * (n/1000.) * (n/1000.)/tcpu );
      else
         flops = 0.0;
      printf ("-------------------------------");
      printf ("matrix dimension is %6i x %6i \n",n,n);
      printf ("largest eigenvalue by Ritz method on %3i processors \n",nproc);
      printf ("eigenvalue after %3i iterations : %20.8f \n",il,evn);
      printf ("compute time ............ : %8.5f sec \n",tmv);
      printf ("communication time ...... : %8.5f sec \n",tcl);
      printf ("speed ................... : %10.2f MFlop/s \n",  flops);
   }
   MPI_Finalize();
   return 0;
}




