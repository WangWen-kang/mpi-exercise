# hello_mpi
from mpi4py import MPI
import numpy as np
import math

numdiv = np.zeros(16,dtype = int)
comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()
nprime = 118845 #118843
ntest = int(math.sqrt(nprime))+1
nd = 0
for i in range(3+rank,ntest,size):
  nr = nprime%i
  if nr == 0 :
    nd = nd+1
    print(rank, ' : ', i)
numdiv[rank]=nd
comm.send(nd,dest=0)
if rank ==0 :
  for ip in range(0,size) :
    numdiv[ip]= comm.recv(source=ip,tag=MPI.ANY_TAG)
  print ('prime number test for ',nprime)
  print(numdiv[0:size])
