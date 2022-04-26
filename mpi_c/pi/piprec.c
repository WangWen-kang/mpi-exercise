#include<math.h>
#include<stdio.h>

void numapp(double lo, double hi, int n, double *res);
  int main(argc,argv)
     int  argc;
     char *argv[];
{ 
  double pia, hi, lo, width;
  double piexact = 3.14159265358979323846264338327950288419, res; 
  int i, n, nin;

/*  input of number of intervals n*/
  printf("\n  number of strips ?  \n");
  scanf("%d",&n);
 printf("\n  number of intervalls ?  \n");
  scanf("%d",&nin);

  printf("\n calculation of pi using numerical integration of SQRT(1-x**2) \n number of strips %d \n number of subintervals    :  %d \n",n, nin);

/* numerical approximation */
  width = 1./nin;
  pia = 0.; 
  for (i = 1 ;i <=nin; i++){
       hi = i * width;
       lo = hi - width;
       numapp(lo, hi, n, &res);
//       printf("\n number of strips : %d",n);
       pia = pia + res;
  }


/*  output of results*/
  printf("\n pi exact          = %20.15f",piexact);
  printf("\n pi estimated      = %20.15f",pia);
  printf("\n error exact       = %20.3e \n",fabs(pia-piexact));
  return 0;
}




