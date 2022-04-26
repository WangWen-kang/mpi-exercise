#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "decls.h"

/*
=================================
  Waermeleitungsgleichung
  Message Passing (MPI-)Version
=================================
*/

int main(int argc, char **argv)
{
   int nmax = 3200;
   int n, nt, i, j1, j2, nop, n1l, ip;
   int nproc, myid, ist, istl;
   double r, t1, t2, speed, tav, tavl;
   double c1, c2, c3, c4, c5, c6;
   double trand, tupd;
   double **a, **u;
   int *firstind;
   FILE *fp;
   int fileOK=0;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

/* ------------ 1. Parametereingabe --------------------------------- */

   if(myid == 0) {
      scanf("%d", &nt);
      scanf("%d", &n);
      scanf("%lf", &r);
      printf("Eingelesene Werte:\n"
             "n = %i,\tnt = %i,\nr = %e\n",
             n, nt, r);
      if(r>0.25)
         printf("Warnung: r = %e ! Stabilitaet nur fuer r < 0.25 !\n", r);
   }

   MPI_Barrier(MPI_COMM_WORLD);
   
   MPI_Bcast(&nt,1,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Bcast(&r,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
   
   firstind = malloc( (nproc+1) * sizeof(int) );
   dist_index(n, firstind, nproc);
   n1l = firstind[myid+1] - firstind[myid];
   
   istl = 0;
   if ( (n1l*n) > (nmax*nmax) ) {
      printf("\nSpeicherplatz fuer Gittergroesse nicht ausreichend:\n");
      printf("nmax = %i,\t n1l = %i,\t n = %i", nmax, n1l, n);
      istl = -1;
   }
   
   MPI_Allreduce( &istl, &ist, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD );
   
   if (istl < 0) {
      MPI_Finalize();
      return 1;
   }

   a = malloc( (n1l+2) * sizeof(double*) );
   u = malloc( (n1l+2) * sizeof(double*) );
   if(! (a && u) ) {
      printf("Error allocating memory.\n");
      return 1;
   }
   a[0] = malloc( (n1l+2) * (n+2) * sizeof(double) );
   u[0] = malloc( (n1l+2) * (n+2) * sizeof(double) );
   if(! (a[0] && u[0])) {
      printf("Error allocating memory.\n");
      return 1;
   }

   for (i = 1; i< (n1l+2); i++) {
      a[i] = a[0] + i*(n+2);
      u[i] = u[0] + i*(n+2);
   }


/* ------------ 2. Initialisierung der Anfangswerte u. Quellen ------- */
   initialisierung ( n1l, n , a );
   initialisierung ( n1l, n , u );
   MPI_Barrier(MPI_COMM_WORLD);

/* ------------ 3. Zeitschritte zur Temperaturfeldberechnung --------- */
   trand = 0.0;
   tupd = 0.0;


   t1 = MPI_Wtime();
   for ( i = 1; i<=nt; i++) {
      c1 = MPI_Wtime();
      randaustausch( n1l, n, a );
      c2 = MPI_Wtime();
      zeitschritt ( r, n1l, n, a, u );
      c3 = MPI_Wtime();
      randaustausch( n1l, n, u );
      c4 = MPI_Wtime();
      zeitschritt ( r, n1l, n, u, a );
      c5 = MPI_Wtime();
      trand = trand + c2-c1 + c4-c3;
      tupd = tupd + c3-c2 + c5-c4;
   }
   tav=0.0;
   tavl = 0.0;
   for ( j2 = 1; j2< n+1; j2++) {
         for ( j1 = 1 ; j1 < n1l+1; j1++) {
            tavl = tavl + a[j1][j2];
//            printf("\n %i %i %f",j1,j2,a[j1][j2]);
         }
   }
                        
   MPI_Reduce(&tavl,&tav,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
//   printf("%4i,average temperature : %e \n", myid,tavl);
//   printf("%4i,average temperature : %e \n", myid,tav);
   t2 = MPI_Wtime() - t1;

/* ------------ 4. Resultatausgabe - --------------------------------- */

   MPI_Barrier(MPI_COMM_WORLD);
   if (nproc==1)  ausgabe ( n1l, n, a );
   if (myid == 0) {
      if ( i > nt ) i = nt;
      printf("\n\n");
      printf("-------------------------------");
      printf("\n\n system size: %i x %i ", n, n);
      printf("\n after %i time steps :", i);
      printf("\n average temperatur : %10.5f ",tav/((double)n*(double)n));
      printf("\n run time %8.4f",t2);
      nop = 6*n*n;
      speed = 1.e-6*nop*2*nt/t2;
      printf("\n run speed [Mflop/s] : %10.2f",speed);
      printf("\n trand, tupd : %8.4f %8.4f \n", trand, tupd);
   }

   MPI_Finalize();

   return 0;
}
