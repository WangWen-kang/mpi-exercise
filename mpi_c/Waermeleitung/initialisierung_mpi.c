#include <mpi.h>
#include <stdio.h>
#include <math.h>

/* Vorbedingung: a ist mdst. ein (n1+2)x(n2+2) grosses Feld */

/* Bemerkung: Verglichen mit der FORTRAN-Version
   sind hier Zeilen/Spalten vertauscht, d.h. wir unterteilen
   in Bloecke von Zeilen! */

void initialisierung(int n1, int n2, double **a)
{
   double anfangswert=1.0, randwert=9.0;
   int j1, j2;
   int nproc, myid;

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

#ifdef DEBUG
   printf("DEBUG: Initialisierung %d\n",myid);
#endif

   for ( j2 = 1; j2<= n2 ; j2++ ) {
      for ( j1 = 1;j1<= n1; j1++ )
         a[j1][j2] = anfangswert;
   }

   /* Root: oberer Rand*/
   if (myid  == 0) {
      for ( j2 = 0; j2<= n2+1; j2++)
         a[0][j2] = randwert; 
/* Alternativ z.B. a[0][j2] = randwert*sin(j2*2*3.1415/n2); */
   }

   /* Letzter proc: unterer Rand */
   if (myid == nproc-1) {
      for ( j2 = 0 ; j2<= n2+1; j2++)
         a[n1+1][j2] = randwert;
      /* Alternativ z.B. a[n1+1][j2] = randwert*cos(j2*2*3.1415/n2); */
   }

   /* Jeder: linker/rechter Rand */
   for ( j1 = 0; j1<= n1+1;j1++) {
      a[j1][0] = randwert;
      a[j1][n2+1] = randwert;
   }
#ifdef DEBUG
   printf("DEBUG: Initialisierung finished %d\n",myid);
#endif

}


/*
Hauptprogramm zu Testzwecken

#include <stdlib.h>

int main(int argc, char **argv) {
   double **a;
   int n1 = 5, n2 = 5, qwe, asd, nproc, myid;
   
   MPI_Init(&argc,&argv);

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);
   
   a = malloc((n1+2) * sizeof(double*));
   for (qwe = 0; qwe < (n1+2); qwe++) {
      a[qwe] = malloc((n2+2) * sizeof(double));
   }
   
   initialisierung(n1, n2, a);
   
   for (qwe = 0; qwe < (n1+2); qwe++) {
      for (asd = 0; asd < (n2+2); asd++)
         printf("%d:%d:%d:%f ", myid,qwe,asd,a[qwe][asd]);
      printf ("\n");
   }
   
   MPI_Finalize();
}

*/
