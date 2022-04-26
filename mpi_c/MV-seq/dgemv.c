void dgemv(int n1,int n2 ,double *a, int lda, double *x, double *y)
{
   int i,j,ia;
   double s0;

   for (i = 0 ;i< n2;i++)
   {
      s0 = 0.;
      ia = lda*i;
      for (j = 0 ; j< n1;j++)
      {
         s0 = s0 + a[j+ia] * x[j];
      }
      y[i+0] = s0;
   }
   return;
}
