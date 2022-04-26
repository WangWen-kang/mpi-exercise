#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "decls.h"

/*
  ===================================================================
  Waermeleitungsgleichung
  Einprozessorprogramm
  ===================================================================
*/

int main (int argc, char **argv)
{
   int nmax=3200, nmax2=nmax+2;
   int n, nt, i, j1, j2, nop, aus, qwe;
   double r, t1, t2, speed, tav;
   double **a, **u;

   MPI_Init(&argc,&argv);
   /* Alloc memory */
   a = malloc ( ( nmax2 + 1 ) * sizeof ( double* ) );
   u = malloc ( ( nmax2 + 1 ) * sizeof ( double* ) );
   for ( qwe = 0; qwe <= nmax2; qwe++) {
      a[qwe] = malloc ( ( nmax2 + 1 ) *sizeof ( double ) );
      u[qwe] = malloc ( ( nmax2 + 1 ) *sizeof ( double ) );
   }

/* ------------ 1. Parametereingabe ---------------- */

   scanf("%d", &nt);
   scanf("%d", &n);
   scanf("%lf", &r);
   scanf("%d", &aus);
   printf("\nEingegebene Werte: \nnt = %d,\tn = %d,"
          "\tr = %f,\taus = %d\n", nt, n, r, aus);

   if (r > 0.25)
      printf("\nWarnung: r = %f ! Stabilitaet nur fuer r < 0.25 !", r);


   if ( ( n*n ) > ( nmax*nmax ) ) {
      printf("\nSpeicherplatz fuer Gittergroesse nicht ausreichend:"
             "\nnmax = %i,\tn = %i\n\n", nmax, n);
      return 1;
   }

/* ------------ 2. Initialisierung der Anfangswerte u. Quellen ------- */



   /* Abfangen von fehlerhaften Eingaben */
   if ( n > nmax2 -1 ) n = nmax2 -1;
   if ( n < 0 ) n = nmax2 -1;
   /* printf("\n%i = n1 und n2 = %i \n ...\n", n1, n2); */

   initialisierung ( n, n , a ); 
   initialisierung ( n, n , u );

/* Feld a ist nun initialisiertes (n1+1)x(n2+1) dimensionales Feld */

/* ------------ 3. Zeitschritte zur Temperaturfeldberechnung --------- */

   t1 = MPI_Wtime();

   for ( i = 1; i<=nt; i++) {
      zeitschritt (r, n, n, a, u);
      zeitschritt (r, n, n, u, a);
   }

   t2 = MPI_Wtime() - t1;


/* ------------ 4. Resultatausgabe - --------------------------------- */
   tav = 0; 
   for ( j2 = 1; j2< n+1; j2++) {
         for ( j1 = 1 ; j1 < n+1; j1++) {
            tav = tav + a[j1][j2];
//            printf("\n %i %i %f",j1,j2,a[j1][j2]);
         }
   }
    ausgabe ( n, n, a );
   printf("\n\n");
   printf("-------------------------------");
   printf("\n\n system size: %i x %i ", n, n);
   printf("\n after %i time steps :", i);
   printf("\n average temperatur : %10.5f ",tav/((double)n*(double)n));
   printf("\n run time %8.4f",t2);
   nop = 6*n*n;
   speed = 1.e-6*nop*2*nt/t2;

   printf("\nRechengeschwindigkeit [Mflop/s] : %10.2f\n",speed);
   MPI_Finalize();

   return 0;
}
