#intsum.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

n = None
if myid == 0:
   print ("n = ?")
   n = int(input())
n = comm.bcast(n,root=0)

# sequential summation of integers from 1 to n:
sum = 0
for i in np.arange(1,n+1,1):
   sum = sum + i

# distribute the summation of integers from 1 to n
# over nproc tasks

if myid == 0:
   print ('sum of i from 1 to',n,' is: ',sum)
   print ('correct result is: ',n*(n+1)/2)
