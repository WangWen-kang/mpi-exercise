# hello_mpi
from mpi4py import MPI
import math

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()
nprime = 118845 #118843
if rank == 0:
  print ('primnumber test for ',nprime)
ntest = int(math.sqrt(nprime))+1
nd = 0
for i in range(3,ntest,1):
  nr = nprime%i
  if nr == 0 :
    nd = nd+1
    print(rank, ' : ', i)
print ('task',rank,' found',nd,'divisors')
