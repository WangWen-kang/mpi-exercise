#include<stdio.h>

void zeitschritt (double r,int n1, int n2, double **a, double **u)
{
   int j1, j2;
   double s;

#ifdef DEBUG
   printf("DEBUG: Zeitschritt\n");
#endif

   s = 1 - 4*r;

   for ( j2 = 1; j2 <= n2 ; j2++ ) {
      for ( j1 = 1; j1 <= n1 ; j1++ ) {
         u[j1][j2] = s*  a[j1][j2]
            + r * ( a[j1-1][j2] + a[j1][j2-1]  + a[j1][j2+1] +a[j1+1][j2]);
      }
   }
}

/*

  int main() {
  double **a, **u, r = 1.5;
  int n1 = 5, n2 = 5, qwe, asd;
  a = malloc((n1+2) * sizeof(double*));
  u = malloc((n1+2) * sizeof(double*));

  for (qwe = 0; qwe <= n1+1; qwe++) {
  a[qwe] = malloc((n2+2) * sizeof(double));
  u[qwe] = malloc((n2+2) * sizeof(double));
  }


  for (qwe = 0; qwe <= n1+1; qwe++) {
  for (asd = 0; asd <= n2+1; asd++) {
  a[qwe][asd] = qwe*asd;
  // 		u[qwe][asd] = qwe+asd;
  }
  }



  zeitschritt(r, n1, n2, a, u);



  for (qwe = 0; qwe <= n1+1; qwe++) {
  for (asd = 0; asd <= n2+1; asd++)
  printf("%lf ", a[qwe][asd]);
  printf("\n");
  }
  printf("\n\nJETZT KOMMT U\n\n");

  for (qwe = 0; qwe <= n1+1; qwe++) {
  for (asd = 0; asd <= n2+1; asd++)
  printf("%lf ", u[qwe][asd]);
  printf("\n");
  }


  return 0;
  }


*/
