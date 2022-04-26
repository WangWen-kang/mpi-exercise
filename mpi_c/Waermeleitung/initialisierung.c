#include <stdio.h>
#include <math.h>

/* Vorbedingung: a ist mdst. ein (n1+1)x(n2+1) grosses Feld */

void initialisierung ( int n1, int n2, double **a )
{
   double anfangswert = 1.0, randwert= 9.0;
   int j1, j2;

#ifdef DEBUG
   printf("DEBUG: Initialisierung!\n");
#endif

   for ( j2 = 1; j2<= n2 ; j2++)
      for ( j1 = 1; j1<= n1; j1++)
         a[j1][j2] = anfangswert;

   for ( j1 = 0; j1 <= n1+1; j1 ++)
      a[j1][0] = randwert;
   
   for ( j1 = 0; j1<= n1+1; j1++ )
      a[j1][n2+1] = randwert;

   for ( j2 = 0 ; j2 <= n2+1; j2++ ) {
      a[0][j2] = randwert;
      a[n1+1][j2] = randwert;
   }
}


/* Hauptprogramm zu Testzwecken

 int main() {
 	double **a;
 	int n1 = 5, n2 = 5, qwe, asd;
 	a = malloc((n1+2) * sizeof(double*));
 	for (qwe = 0; qwe < (n1+2); qwe++) {
 		a[qwe] = malloc((n2+2) * sizeof(double));
 	}
 	initialisierung(n1, n2, a);

 	for (qwe = 0; qwe < (n1+2); qwe++) {
 		for (asd = 0; asd < (n2+2); asd++)
 			printf("%lf ", a[qwe][asd]);
 	printf ("\n");
 	}


 	return 0;
 }

*/
