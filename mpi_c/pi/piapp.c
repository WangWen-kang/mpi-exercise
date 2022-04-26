#include <math.h>
#include <stdio.h>

  main(argc,argv)
     int  argc;
     char *argv[];
{ 
  double del, app, xi;
  double pia, piexact = 3.14159265358979323846264338327950288419; 
  int n, i;
/*  input of number of strips */
  printf("\n  number of strips ?  \n");
  scanf("%d",&n);
  printf("\n calculation of pi using numerical integration of SQRT(1-x**2) \n using  %d strips \n",n);

/* numerical approximation */
  del = 1./((double) n);
  app = 0.;
  for (i=0;i<n;i++){
    xi = (i+0.5)*del;
    app = app + sqrt(1.-xi*xi);
  }
  pia = 4.*del*app;

/*  output of results*/
  printf("\n pi exact          = %20.15f",piexact);
  printf("\n pi estimated      = %20.15f",pia);
  printf("\n error             = %20.3e \n",pia-piexact);
  return 0;
}




