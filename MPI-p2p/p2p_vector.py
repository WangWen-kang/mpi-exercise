
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

vec = myid*np.arange(1,11,dtype = np.float64)
print (myid, '>> initial:  vec=', vec)

if myid == 1:
   nsend = 3
   startsend = 4
   comm.Send(vec[startsend:startsend+nsend],dest=0,tag=11)
if myid == 0:
   nrecv = 3
   startrecv = 1
   comm.Recv(vec[startrecv:startrecv+nrecv],source=MPI.ANY_SOURCE,tag=MPI.ANY_TAG)
   print (myid, '>> nach recv: vec=', vec)



