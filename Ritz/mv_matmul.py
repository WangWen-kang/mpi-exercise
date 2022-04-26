import math
import time
import numpy as np

def dgemv(scalar,mat,vec):
   x = np.matmul(mat,vec)
   return x
