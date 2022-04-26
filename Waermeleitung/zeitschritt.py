import math
import time
import numpy as np

def zeitschritt(r,a,u):
   (n1,n2) = a.shape
   s = 1. - 4.*r
   for j2 in range(0,n2-2):
      for j1 in range(0,n1-2):
#         print ("%2.1f %2.1f %2.1f %2.1f %2.1f" %(a[j1+1,j2+1],a[j1,j2+1], a[j1+2,j2+1],a[j1+1,j2],a[j1+1,j2+2]))
         u[j1+1,j2+1] =  s*  a[j1+1,j2+1] + r * (                         \
                                   a[j1,j2+1] +                   \
                a[j1+1,j2]  +                       a[j1+1,j2+2] +  \
                                   a[j1+2,j2+1]                    )
   return u                      
