import math
import time
import numpy as np

def mv(scalar,mat,vec):
   (n1,n2) = mat.shape
   x = np.zeros(n1)
   for i2 in np.arange(n2):
      for i1 in np.arange(n1):
         x[i1] = x[i1] + mat[i1,i2]*vec[i2]
   return x
