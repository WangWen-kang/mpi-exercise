#include<math.h>
void numapp(double lo, double hi, int n, double *pres){
  double res, app, xi, xim, del, flo, fhi, fi, fim;
  int i, nl;
  flo =  sqrt(1.-lo*lo); fhi =  sqrt(1.-hi*hi);
  nl = n*(flo-fhi)+1;
  del = (hi-lo)/((double) nl);
  app = 0.; 
  for (i=0;i<nl;i++){
    xi = lo + (i+0.5)*del;
    app = app + sqrt(1.-xi*xi);
  }
  *pres = 4.*del*app;
  return;
}





