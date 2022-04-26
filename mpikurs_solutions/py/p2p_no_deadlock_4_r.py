#p2p_deadlock.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

for i in np.arange(1,9):
   nl = 10**i
   vec = myid*np.ones(nl,dtype = np.int) 
   rvec = np.empty(nl,dtype = np.int) 

   print ('size ',nl,'start value ',myid, '>> ',vec[1])
   ipto = (myid +1)%nproc
   ipfr = (myid-1)%nproc
   req = comm.Irecv(rvec,source=ipfr)
   comm.Send(vec,dest=ipto)
   MPI.Request.Wait(req)
   print ('size ',nl,'value after ring shift',myid, '>> ',rvec[1])


