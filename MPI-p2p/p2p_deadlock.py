#p2p_deadlock.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

for i in np.arange(1,8):
   nl = 10**i
   vec = myid*np.ones(nl,dtype = int) 
   rvec = np.empty(nl,dtype = int) 

   print (myid, '>>   size ',nl,'start value ',vec[1])
   ipto = (myid +1)%nproc
   ipfr = (myid-1)%nproc
   comm.Send(vec,dest=ipto)
   comm.Recv(rvec,source=ipfr)
   print (myid, '>>   size ',nl,'value after ring shift',rvec[1])


