
/*
 +-------------------------------------------------------------------+
 |                                                                   |
 |  This program determines the largest eigenvalue of a matrix using |
 |  the method of Raleigh and Ritz.                                  |
 |                                                                   |
 |  Sequential version                                               |
 +-------------------------------------------------------------------+
 */
 
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ritz.h"

int main(int argc,char **argv)
{ 

   int nmax= pow(2,13);
   int i, j, n, il, nloop=1000, istop;
   double *a, *x, *y; 
   double flops, tcpu, evn, evo, res;
   double tmv, tcl, tte, tsc;
   
   a = malloc(nmax*nmax*sizeof(double));
   x = malloc(nmax*sizeof(double));
   y = malloc(nmax*sizeof(double));

   if(! (a && x && y) ) {
      printf("Error allocating memory.");
      return -1;
   }

   MPI_Init(&argc,&argv);
   
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

   printf(" \n Initializing data ...\n");
   for (j = 1; j <= n;j++){
      for (i = 1; i < j;i++){
         a[i-1 + n * (j - 1)] = a[j-1 + n * (i - 1)];
      }
      for (i = j; i <= n;i++){
         a[i-1 + n * (j - 1)] = ( (double)i + (double)j ) / (double)i;
      }
   }
/*
  --------------------------------------------------------------
  Raleigh-Ritz algoritm
  --------------------------------------------------------------
*/
   for (i = 1; i <= n;i++) 
      x[i-1] = 1.0;

   tmv = 0.0;
   tcl = 0.0;
   tte = 0.0;
   tsc = 0.0;
   tcpu = MPI_Wtime();
  
   evo = x[0];
   istop = 0;
   for (il = 1; il <= nloop;il++){
      mv(n, n , a, x, y);
      evn = y[0];
      res = (evn - evo) / evn;
      if (fabs(res) < 1.0e-12) istop = 1;
      evo = evn;
      printf("%d %20.15f %e \n",il,evn,res);
      if (istop == 1) break;
      for (i = 1; i <= n;i++){
         x[i-1] = y[i-1] / evn;
      }
   }

   tcpu = MPI_Wtime() - tcpu;

   if (tcpu > 0.0)
      flops = 1e-6 * (2 * il * (double)n * (double)n) / tcpu;
   else
      flops = 0.0;
   printf(" Eigenvalue after %d iterations ..... : %f \n", il, evn);
   printf(" MFlop/s ............................ : %f \n", flops);
   printf(" CPU seconds ........................ : %f \n", tcpu);
   MPI_Finalize();
   return 0;
}




