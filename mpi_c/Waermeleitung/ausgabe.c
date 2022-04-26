#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

/* Gibt ersten n1 Zeilen und n2 Spalten von **u aus
   Vorbedingung: Die entsprechenden Eintraege wurden fuer **u korrekt allokiert */

void ausgabe ( int n1, int n2, double** u ) {
/*       real*8           u(n1,n2)  */
   int j1, j2;

      int nskip1  = (n1+1)/20;
      if (nskip1==0) nskip1 = 1;
      int nskip2  = (n2+1)/20;
      if (nskip2==0) nskip2 = 1;
//      printf("%4d %4d %4d %4d \n",n1,n2,nskip1,nskip2);
   for (j1 = 0; j1 <= n1+1; j1=j1+nskip1) {
      for (j2 = 0; j2 <= n2+1; j2=j2+nskip2){
         printf("%4.1f", u[j1][j2]);
      }
      printf("\n");
   }
}
