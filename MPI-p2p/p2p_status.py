#p2p_vector.py
from mpi4py import MPI
import numpy as np

#  create status data structure:
stat = MPI.Status()

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
   nrecv = 5
   print ('number of elements to be received: ',nrecv)
   startrecv = 1
#  include status argument "stat" 
   comm.Recv(vec[startrecv:startrecv+nrecv],source= MPI.ANY_SOURCE,tag=MPI.ANY_TAG,status=stat)
# retrieve number of received elements:
   nrecv = stat.Get_elements(MPI.DOUBLE)
   print ('number of received elements : ',nrecv)
   print (myid, '>> nach recv: vec=', vec)



