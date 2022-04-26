#zeit_mpi.py
from mpi4py import MPI
import numpy as np

tick = MPI.Wtick()
n = 100
t = np.zeros(n) 
for i in range(0,n):
   t[i] = MPI.Wtime() 

print ('precision of Wtime:',tick,'sec')
print ('time for Wtime    :',(t[n-1]-t[2])/(n-2),'sec')

