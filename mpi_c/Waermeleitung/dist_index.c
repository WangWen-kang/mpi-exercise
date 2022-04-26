#include <stdio.h>

void dist_index( int n, int *firstind , int nproc)
{
   int  nl, ip;
   
#ifdef DEBUG
   printf("DEBUG: dist_index\t%d\n", nproc);
#endif

   nl = (n + nproc-1) / nproc;
   firstind[0] = 1;
   for (ip = 1; ip <= nproc; ip++)
      firstind[ip] = n+1 < firstind[ip-1]+nl ? n+1 : firstind[ip-1]+nl;
}
