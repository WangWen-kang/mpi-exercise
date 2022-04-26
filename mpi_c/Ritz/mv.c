void mv(int n1,int n2 ,double *a, double *x, double *y)
{
   int i,j,ja;

   for (i = 0 ;i< n1;i++)
      y[i] = 0;
   for (i = 0 ;i< n1;i++){
      for (j = 0 ; j< n2;j++){
         ja = n1*j;
         y[i] =  y[i]  + a[i+ja] * x[j];
      }
   }
   return;
}
