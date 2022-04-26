import math
import time
import numpy as np
from scipy.linalg.blas import dgemv

# matrix size
n = int(input('n = ?   '))
# maximal number of iterations
nloop = 100
# required precision
eps = 1.e-12
# matrix initialization
a = np.zeros((n,n))
for i in np.arange(n):
   for j in np.arange(i,n):
      a[i,j] = np.float64(i+j+2)/np.float64(j+1)
   for j in np.arange(i):
      a[i,j] = a[j,i]
#initialise start vector
x = np.ones(n)
t = time.time()
evo = x[0]
istop = 0
for i in np.arange(nloop):
   y = dgemv(1,a,x)
#   y = np.dot(x,a)
   evn = y[0]
   res = (evn-evo)/evn
   evo = evn
   print (i,evn,res)
   if abs(res) < eps:
      break
   x = y/evn
tcpu = time.time() -t
flops = 1.e-6 * float(2 * (i+1) * n * n) / tcpu
print ("cpu time [s] : %10.2e , speed [Mflop/s] : %20.2e " % (tcpu,flops))
